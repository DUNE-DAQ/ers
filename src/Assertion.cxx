/*
 *  Assertion.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include <iostream>
#include <sstream>

#include "ers/Assertion.h"
#include "ers/Issue.h"


namespace {
    ers::Issue *create_issue() { return new ers::Assertion(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::Assertion::ASSERTION_CLASS_NAME,create_issue) ;
} 

/** Key for assertion condition description 
 */

const char *ers::Assertion::ASSERT_CONDITION_KEY = "ASSERTION_CONDITION" ; 

const char *ers::Assertion::ASSERTION_CLASS_NAME = "ers::Assertion" ;


/** Empty constructor for deserialisation / factory 
  */

ers::Assertion::Assertion() : Issue() {} 

/** Constructor for subclasses 
  */

ers::Assertion::Assertion(ers_severity s, const Context &context) : Issue(s,context) {} 

/** Constructor 
  * @param condition_text the text of the assertion's condition
  * @param message message describing the assertion's condition 
  * @param s severity of the assertion 
  * @param context position in the code where the assertion failed, should be the MRS_HERE macro 
  * @param constant_expression is the expression constant (as detected by the compiler).
  */

ers::Assertion::Assertion(const char* condition_text, const std::string &message, ers_severity s, const Context &context, bool constant_expression) : Issue(s,context) {
    setup(condition_text,message,constant_expression);
} // Assertion

/** Setup method, should be called at the end of the constructor by class and subclasses 
  */

void ers::Assertion::setup(const char *condition_text, const std::string &message, bool constant_expression)  {
    m_value_table[ASSERT_CONDITION_KEY] = *condition_text ;
    responsibility(ers_resp_server); 
    transience( ! constant_expression); 
    finish_setup(build_message(condition_text,message,constant_expression));
} // setup

const char* ers::Assertion::get_class_name() const throw (){
    return ASSERTION_CLASS_NAME ; 
} // get_class_name

/** Builds the message associated with an assertion 
  * @return message describing the failure of the assertion 
  * @param condition_text the condition in textual form 
  * @param message the message associated with the condition
  * @return an human readable message 
  */

std::string ers::Assertion::build_message(const char* condition_text, const std::string &message, bool constant_expression) {
    std::ostringstream m ;
    m << "Assertion '" << condition_text << "' failed: " << message ;
    if (constant_expression) {
	m << " (this condition is constant)" ; 
    } // constant expression 
    return m.str();
} // build_message

