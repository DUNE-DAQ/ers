/*
 *  AllocIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 07.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/AllocIssue.h"
#include "ers/File.h"

#include <sstream>




const char * ers::AllocIssue::CLASS_NAME = "ers::AllocIssue" ;
const char * ers::AllocIssue::POSIX_MALLOC_SIZE_KEY = "POSIX_MALLOC_SIZE" ; 

namespace {
    ers::Issue *create_issue() { return new ers::AllocIssue();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::AllocIssue::CLASS_NAME,create_issue) ;
} 

ers::AllocIssue::AllocIssue() : PosixIssue() {} 

ers::AllocIssue::AllocIssue(const Context &c, size_t size) : PosixIssue(c,ers_error) {
    requested_memory(size); 
    transience(true); 
    setup_message(size); 
} // AllocIssue

ers::AllocIssue::AllocIssue(const Context &c, size_t number, size_t size) : PosixIssue(c,ers_error) {
    const size_t s = number*size ; 
    requested_memory(s); 
    transience(true); 
    setup_message(s); 
} // AllocIssue

void ers::AllocIssue::requested_memory(size_t size) {
    if (size>0) {
	set_value(POSIX_MALLOC_SIZE_KEY,size);
    } // size
} // memory

void ers::AllocIssue::setup_message(size_t size) {
    std::ostringstream message ;
    message << "Cannot allocate" ; 
    if (size>0) {
	message << " " << ers::File::pretty_size(size,true) ;
    } // size is defined
    finish_setup(message.str()); 
} // setup_message

const char *ers::AllocIssue::get_class_name() const throw() {
    return ers::AllocIssue::CLASS_NAME ; 
} // get_class_name