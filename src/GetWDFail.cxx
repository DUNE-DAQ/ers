/*
 *  GetWDFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/GetWDFail.h"

const char * ers::GetWDFail::CLASS_NAME = "ers::GetWDFail" ;

namespace {
    ers::Issue *create_issue() { return new ers::GetWDFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::GetWDFail::CLASS_NAME,create_issue) ;
} 

ers::GetWDFail::GetWDFail() : IOIssue() {} 
ers::GetWDFail::GetWDFail(const Context &c) : IOIssue(c,ers_error) {}

const char *ers::GetWDFail::get_class_name() const throw() {
    return ers::GetWDFail::CLASS_NAME ; 
} 

