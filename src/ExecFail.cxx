/*
 *  ExecFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/ExecFail.h"

#include "ers/File.h"

#include <sstream>

const char * ers::ExecFail::CLASS_NAME = "ers::ExecFail" ;

namespace {
    ers::Issue *create_issue() { return new ers::ExecFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::ExecFail::CLASS_NAME,create_issue) ;
} 

ers::ExecFail::ExecFail() : FileIssue() {} 

ers::ExecFail::ExecFail(const Context &c,const char *p) : FileIssue(c,ers_error,p) {
    std::ostringstream message ;
    message << "Cannot execute" ; 
    setup_file_issue(message.str(),p);
} // UnlinkFail

const char *ers::ExecFail::get_class_name() const throw() {
    return ers::ExecFail::CLASS_NAME ; 
} // UnlinkFail

