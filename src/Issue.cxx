/*
 *  Issue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include <iostream>
#include <sstream>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "ers/Issue.h"
#include "ers/HumanStream.h"
#include "ers/Precondition.h"
#include "ers/InvalidReferenceIssue.h"
#include "system/File.h"
#include "system/Environnement.h"

#define BUFFER_SIZE 256 

/** This block insert the relevant function into the Issue Factory table.
  * This is needed to ensure that serialisation classes work 
  */

namespace {
    ers::Issue *create_issue() { return new ers::Issue(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::Issue::ISSUE_CLASS_NAME,create_issue) ;
} 

using namespace ers ; 
using namespace System  ;

const char* Issue::CLASS_KEY = "ISSUE_CLASS" ; 
const char* Issue::COMPILATION_TIME_KEY = "COMPILATION_TIME" ; 
const char* Issue::COMPILATION_TARGET_KEY = "COMPILATION_TARGET" ; 
const char* Issue::COMPILER_KEY = "COMPILER" ; 
const char* Issue::CPP_CLASS_KEY = "ISSUE_CPP_CLASS" ; 
const char* Issue::ERS_VERSION_KEY = "ERS_VERSION" ;
const char* Issue::HOST_NAME_KEY = "HOST_NAME" ; 
const char* Issue::HOST_TYPE_KEY = "HOST_TYPE" ; 
const char* Issue::MESSAGE_KEY = "MESSAGE" ; 
const char* Issue::PROCESS_ID_KEY = "PROCESS_ID" ;
const char* Issue::PROCESS_PWD_KEY = "PROCESS_PWD" ; 
const char* Issue::PROGRAM_NAME_KEY = "PROGRAM_NAME"; 
const char* Issue::RESPONSIBILITY_KEY = "RESPONSIBILITY" ; 
const char* Issue::SEVERITY_KEY = "SEVERITY" ; 
const char* Issue::SOURCE_POSITION_KEY = "SOURCE_POSITION" ; 
const char* Issue::TIME_KEY = "TIME" ; 
const char* Issue::TRANSIENCE_KEY = "TRANSIENCE" ; 
const char* Issue::USER_ID_KEY = "USER_ID" ; 
const char* Issue::USER_NAME_KEY = "USER_NAME" ; 
const char* Issue::CAUSE_TEXT_KEY = "CAUSE_TEXT"  ;
const char* Issue::CAUSE_PSEUDO_KEY = "CAUSE" ; 


const char* Issue::ISSUE_CLASS_NAME = "ers::issue" ; 

// Constructors
// ====================================================





/** Empty constructor, should only be used when deserialising issues 
  */

Issue::Issue() {
    m_human_description = "" ; 
    cause(); 
} // Issue

Issue::Issue(const Issue &issue){
    this->m_human_description = issue.m_human_description ;
    this->m_value_table = issue.m_value_table ; 
} // Issue

/** Builds an Issue out of a value table
  * \param values table of values for the issue
  */

Issue::Issue(const string_map_type &values) {
    cause(); 
    set_values(values); 
    m_human_description = this->get_value(MESSAGE_KEY) ; 
} // Issue


/** Constructor 
* \param m message of the Issue
* \param s severity of the Issue 
* \param file file where Issue occured 
* \param line line where Issue occured 
*/

Issue::Issue(const Context &context, ers_severity s, const std::string &m) {
    cause(); 
    setup_common(&context);
    severity(s);
    finish_setup(m); 
} // Issue

/** @overload */

Issue::Issue(const Context &context, ers_severity s) {
    cause();   
    setup_common(&context);
    severity(s);
} // Issue

/** Constructor - takes another exceptions as the cause for the current exception.
 * 
 * \param s the severity of the exception
 * \param cause the exception that caused the current Issue
 */

Issue::Issue(const Context &context, ers_severity s, const std::exception *cause_exception) {
    ERS_PRE_CHECK_PTR(cause_exception);
    cause(cause_exception); 
    setup_common(&context);
    severity(s);
    finish_setup(cause_exception->what()); 
} // Issue

Issue *Issue::clone() const { 
    return IssueFactory::instance()->build(this) ; 
} // clone


// Value Table manipulation Methods 
// ====================================================

/** Returns a read-only pointer to the value table
  * @return read only pointer to the table
  */

const string_map_type* Issue::get_value_table() const {
    return &m_value_table ; 
} // get_value_table

/** General method for querying properties of the Issue 
* \param key the key to lookup
* @return the string value for the key and empty string if the key is not found
*/

const std::string Issue::get_value(const std::string &key) const {
    string_map_type::const_iterator pos = m_value_table.find(key);
    if (pos!=m_value_table.end()) {
        return pos->second ; 
    }
    return "";
} // get_value

int Issue::values_number() const {
    return m_value_table.size(); 
} // values_number


/** Sets the value table 
 * \param value the value table to load
 */

void Issue::set_values(const string_map_type &values) {
    m_value_table = values ;
} // load_values

/** Set a numerical value in the value table
  * \param key the key to insert
  * \param value the value to insert
  */


void Issue::set_value(const std::string &key, long value) {
    std::ostringstream stream ;
    stream << value ; 
    m_value_table[key] = stream.str();
} // set_value

// ====================================================
// Insertions Methods
// ====================================================

/** Inserts the context of the issue into the issue 
* \param context pointer to context object
*/

void Issue::insert(const Context *context) {
    ERS_PRE_CHECK_PTR(context); 
    m_value_table[SOURCE_POSITION_KEY] = context->position();
    m_value_table[COMPILER_KEY] = context->compiler();
    m_value_table[COMPILATION_TIME_KEY] = context->compilation(); 
    m_value_table[COMPILATION_TARGET_KEY] = context->host_type(); 
} // insert

/** Inserts the current hostname into the issue  
*/

void Issue::insert_hostname() {
    char host_buffer[BUFFER_SIZE] ;
    int status = gethostname(host_buffer,BUFFER_SIZE);
    if (0==status && (strlen(host_buffer)>0)) {
        m_value_table[HOST_NAME_KEY] = std::string(host_buffer) ;
    } // gethostnameworked
} // insert_hostname

/** Inserts the current process id into the issue
*/

void Issue::insert_processid() {
    std::ostringstream pid_str ;
    pid_str << getpid() ; 
    m_value_table[PROCESS_ID_KEY] = pid_str.str(); 
} // insert_processid

void Issue::insert_userid() {
    std::ostringstream uid_str ;
    uid_str << getuid();
    m_value_table[USER_ID_KEY] = uid_str.str(); 
    insert_env("LOGNAME",USER_NAME_KEY); 
} // insert_userid

/** Inserts the current time into the issue 
*/

void Issue::insert_time() {
    time_t now ;
    time(&now); 
    char time_buffer[BUFFER_SIZE] ; 
    ctime_r(&now,time_buffer) ; 
    char *cr = strchr(time_buffer,'\n');
    if (cr) {
	*cr = '\0' ;
    }
    m_value_table[TIME_KEY] =  time_buffer ; 
} // insert_time

/** Inserts the current working directory 
  * This method should be called once all posix stuff is handled, as it accesses errno 
  */

void Issue::insert_pwd() {
    m_value_table[PROCESS_PWD_KEY] = System::File::working_directory() ; 
} // insert_pwd

/** Inserts a environnement variable into the issue 
* \param env_key name of the environnement variable
* \param issue_key key used to store the resulting value into the value table 
*/

void Issue::insert_env(const char*env_key, const char* issue_key) {
    ERS_PRE_CHECK_PTR(env_key);
    ERS_PRE_CHECK_PTR(issue_key);
    std::string value = Environnement::get(std::string(issue_key)); 
    if (value!=Environnement::NO_VALUE) {
	m_value_table[issue_key] = value; 
    } // value exists 
} // insert_env

// ====================================================
// Setup Methods
// ====================================================

/** This method sets up common fields for all Issues. 
* In particular, it inserts all data concerning the Issue's context, this includes
* \li Source code position (file/line)
* \li Compiler version
* \li Compilation time and date
* \li Hostname 
* \li Process id
* \li OS and processor of the host
*
* \param context context where the exception occured, this should be the ERS_HERE macro. 
*/

void Issue::setup_common(const Context *context) {
    if (context) {
        insert(context);
    } // context
    insert_hostname(); 
    insert_processid(); 
    insert_time();
    insert_userid(); 
    insert_env("HOSTTYPE",HOST_TYPE_KEY); 
} // setup_common

/** Finishes the setting up of the information fields.
  * In particular, in fills in the human message and the class type fields 
  * (those fields are not available until the end of the object construction.
  * @note this method should be called by the sub-class constructor, so that RTTI information is setup and correct. 
  * \param message human readable message 
  */

void Issue::finish_setup(const std::string &message) {
    Issue *p = this ; 
    std::string type_string = (typeid(*p)).name() ; 
    m_value_table[CPP_CLASS_KEY]=type_string ;
    m_value_table[CLASS_KEY] = get_class_name(); 
    m_value_table[MESSAGE_KEY] = message ; 
    m_human_description=build_human_description();
    insert_pwd(); 
} // finish_setup

/** Builds a human readable description of the Issue.
* This is done using the human_Stream class. 
* @return A string containing the human description. 
* @see Human_Stream
*/

std::string Issue::build_human_description() const {
    HumanStream s ; 
    s.send(this);
    return s.str(); 
} // buildmm_human_description

// ====================================================
// Field Access Methods
// ====================================================

/** Returns the key used to describe this particular class when serializing 
  */

const char*Issue::get_class_name() const throw()  {
    return ISSUE_CLASS_NAME ; 
} // get_class_name


/** Gets the severity of the Issue 
 * @return severity of the Issue 
 */

ers_severity Issue::severity() const throw() {
    std::string value = get_value(SEVERITY_KEY); 
    return parse_severity(value);
} // severity

/** Set the severity of the Issue 
 * \param s the severity level 
 */

void Issue::severity(ers_severity s) {
    m_value_table[SEVERITY_KEY] = get_severity_text(s) ; 
} // severity

bool Issue::is_error() {
    ers_severity s = severity(); 
    return (s==ers_error || s== ers_fatal) ;
} // is_error


std::string Issue::severity_message() const {
    return get_value(SEVERITY_KEY);  
} // severity_message


/** Gets the responsibility type of the Issue 
* @return the responsibiliy value of the Issue 
*/

ers_responsibility Issue::responsibility() const throw() {
    std::string value = this->get_value(RESPONSIBILITY_KEY); 
    return parse_responsibility(value);
} // responsability

/** Sets the responsbility of the Issue
* \param r the responsibility type
*/

void Issue::responsibility(ers_responsibility r) {
    m_value_table[RESPONSIBILITY_KEY] = get_responsibility_text(r) ; 
} // responsability


/** Sets the transience of the issue 
  * \param tr true if the issue is transient, false if not
  */

void Issue::transience(bool tr) {
    m_value_table[TRANSIENCE_KEY] = get_boolean(tr);
} // 

/** @return the transience of the issue, 1 = true, 0 = false, -1 = unknown 
  */

int Issue::transience() const throw () {
    std::string value = this->get_value(TRANSIENCE_KEY); 
    return parse_boolean(value.c_str());
} // transience


const std::exception *Issue::cause() const throw() {
    return 0 ; 
} // cause

void Issue::cause(const std::exception *c) {
    if (c==0) {
	return ; 
    } // No cause easy. 
    m_value_table[CAUSE_TEXT_KEY] = c->what(); 
} // cause

/** 
 * @return human description of the Issue 
 */

const std::string Issue::human_description() const throw()  {
    return m_human_description ; 
}  // human_description

/** This method overides the what method of the std::exception class.
  * As this method is declared const, it has to use a pre-calculated string
  * @return C style string of human_description 
  */

const char *Issue::what() const throw() {
    return m_human_description.c_str(); 
} // what();  


Issue::operator std::string() const {
    return m_human_description ;
} // std::string()

// ====================================================
// Stream Operators
// ====================================================


/** Standard Streaming operator - puts the human description into the Stream. 
  * \param Stream the destination Stream 
  * \param Issue the Issue to Stream
  * @see Issue::human_description()
  */

std::ostream& ers::operator<<(std::ostream& s, const Issue& i) {
    return s << i.human_description() ; 
} // operator<<

/** Sends the Issue into a Stream 
  * \param Stream the Stream to send the Issue into
  * \param Issue the Issue to send
  * @return the Stream
  * @see serialize_to() 
  */

Stream& ers::operator<<(Stream& s, const Issue& i) {
    s.send(&i); 
    return s ; 
} // operator<< 




