/*
 *  FStatFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/FStatFail.h"


const char * ers::FStatFail::CLASS_NAME = "ers::FStatFail" ;

namespace {
    ers::Issue *create_issue() { return new ers::FStatFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::FStatFail::CLASS_NAME,create_issue) ;
} 

ers::FStatFail::FStatFail() : FileIssue() {} 
ers::FStatFail::FStatFail(const Context &c,const char *p) : FileIssue(c,ers_error,p) {
    setup_file_issue("cannot get file statics",p);
} // FStatFail

const char *ers::FStatFail::get_class_name() const throw() {
    return ers::FStatFail::CLASS_NAME ; 
} 
