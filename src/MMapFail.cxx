/*
 *  MMapFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 07.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/MMapFail.h"

const char * ers::MMapFail::CLASS_NAME = "ers::MMapFail" ;

namespace {
    ers::Issue *create_issue() { return new ers::MMapFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::MMapFail::CLASS_NAME,create_issue) ;
} 

const char *ers::MMapFail::get_class_name() const throw() { return ers::MMapFail::CLASS_NAME ; } 

ers::MMapFail::MMapFail() : IOIssue() {} 
ers::MMapFail::MMapFail(const Context &c, size_t size, int prot, int flag, int fd, size_t offset) : IOIssue(c,ers::ers_error) {
    file_descriptor(fd);
    finish_setup("cannot map memory region");
} // MMapFail
