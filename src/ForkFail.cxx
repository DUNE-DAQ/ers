/*
 *  ForkFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/ForkFail.h"

#include <sstream>

const char * ers::ForkFail::CLASS_NAME = "ers::ForkFail" ;

namespace {
    ers::Issue *create_issue() { return new ers::ForkFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::ForkFail::CLASS_NAME,create_issue) ;
} 

ers::ForkFail::ForkFail() : PosixIssue() {} 

ers::ForkFail::ForkFail(const Context &c) : PosixIssue(c,ers_error) {
    finish_setup("fork failed");
} // UnlinkFail

const char *ers::ForkFail::get_class_name() const throw() {
    return ers::ForkFail::CLASS_NAME ; 
} // UnlinkFail