/*
 *  IOIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 14.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/IOIssue.h"
#include <sstream>
#include "ers/Precondition.h"

const char* ers::IOIssue::IO_ISSUE_CLASS = "ers::IOIssue" ; 

namespace {
    ers::Issue *create_io_issue() { return new ers::IOIssue();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::IOIssue::IO_ISSUE_CLASS,create_io_issue) ;
} 

ers::IOIssue::IOIssue() : ers::PosixIssue() {} 

ers::IOIssue::IOIssue(const Context &c, ers_severity s) : PosixIssue(c,s) {} 


ers::IOIssue::IOIssue(const Context &c, ers_severity s, const std::string &message) : ers::PosixIssue(c,s,message) {} 
ers::IOIssue::IOIssue(const Context &c, ers_severity s, const std::string &message, const std::exception *cause) : ers::PosixIssue(c,s,message,cause) {}
ers::IOIssue::IOIssue(const Context &c, ers_severity s, const std::exception *cause) : ers::PosixIssue(c,s,cause) {} 

/** Constructor - builds a IOIssue out of a STL exception 
  */

ers::IOIssue::IOIssue(const Context &c, ers_severity s, const std::string &message, const std::ios_base::failure *f) : ers::PosixIssue(c,s,f) {
    ERS_PRECONDITION(f!=0,"ios_base::failure is null"); 
    std::ostringstream full_message_str ;
    full_message_str << message << ", " << severity_message() << ":" << f->what() << ": " << posix_message() ; 
    this->finish_setup(full_message_str.str()); 
} // IOIssue

const char *ers::IOIssue::get_class_name() const throw() {
    return ers::IOIssue::IO_ISSUE_CLASS ; 
}// get_class_name

