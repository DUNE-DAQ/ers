/*
 *  UnlinkFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/UnlinkFail.h"

const char * ers::UnlinkFail::CLASS_NAME = "ers::UnlinkFail" ;

namespace {
    ers::Issue *create_issue() { return new ers::UnlinkFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::UnlinkFail::CLASS_NAME,create_issue) ;
} 

ers::UnlinkFail::UnlinkFail() : FileIssue() {} 
ers::UnlinkFail::UnlinkFail(const Context &c,const char *p) : FileIssue(c,ers_error,p) {
    setup_file_issue("cannot unlink",p);
} // UnlinkFail

const char *ers::UnlinkFail::get_class_name() const throw() {
    return ers::UnlinkFail::CLASS_NAME ; 
} // UnlinkFail


