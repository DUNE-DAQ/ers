/*
 *  FileIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>
#include "ers/FileIssue.h"

const char* ers::FileIssue::PATH_KEY = "POSIX_FILE_PATH" ; 
const char* ers::FileIssue::FILE_TYPE_KEY = "FILE_TYPE" ; 

const char* ers::FileIssue::FILE_ISSUE_CLASS = "ers::FileIssue" ; 

namespace {
    ers::Issue *create_file_issue() { return new ers::FileIssue();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::FileIssue::FILE_ISSUE_CLASS,create_file_issue) ;
} 

ers::FileIssue::FileIssue() : ers::IOIssue() { } // FileIssue

ers::FileIssue::FileIssue(const Context &c, ers_severity s, const char *p) : ers::IOIssue(c,s) {
    transience(false); 
    path(p); 
}  // FileIssue

ers::FileIssue::FileIssue(const Context &c, ers_severity s, const char *p, const std::exception *cause) : ers::IOIssue(c,s,cause) {
    transience(false); 
    path(p); 
} // FileIssue


ers::FileIssue::FileIssue(const Context &c, ers_severity s, const std::string &message, const char *p) : ers::IOIssue(c,s) {
    setup_file_issue(message,p); 
} // FileIssue

ers::FileIssue::FileIssue(const Context &c, ers_severity s, const std::string &message, const char *p, const std::exception *cause) : ers::IOIssue(c,s,cause) {
    setup_file_issue(message,p); 
} //  FileIssue

/** Does the setup of the actual message for file issue 
  * \param message description of the issue (without any severity related term like error).
  * \param p the path of the file that cause the issue
  */

void ers::FileIssue::setup_file_issue(const std::string &message, const char* p) {
    path(p); 
    transience(false); 
    std::ostringstream full_message_str ;
    full_message_str << message << ", " << severity_message() << " for file '" << p << "': " << posix_message() ; 
    this->finish_setup(full_message_str.str()); 
} // setup_file_issue


void ers::FileIssue::path(const char* p) {
    if (p) {
	m_value_table[PATH_KEY] = p ; 
    } // if 
} // set_path

 std::string ers::FileIssue::path() const {
    return get_value(PATH_KEY) ;
} // path

const char *ers::FileIssue::get_class_name() const throw() { 
    return FILE_ISSUE_CLASS ; 
} // get_class_name

