/*
 *  InvalidReferenceIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/InvalidReferenceIssue.h"
#include <sstream>

const char *ers::InvalidReferenceIssue::CLASS = "ers::InvalidReferenceIssue" ; 

const char *ers::InvalidReferenceIssue::REFERENCE_VALUE_KEY = "REFERENCE_VALUE" ; 
const char *ers::InvalidReferenceIssue::REFERENCE_TYPE_KEY = "REFERENCE_TYPE" ; 
const char *ers::InvalidReferenceIssue::REFERENCE_NAME_KEY = "REFERENCE_NAME" ; 
const char *ers::InvalidReferenceIssue::REFERENCE_C_PTR_TYPE = "C Pointer" ; 

namespace {
    ers::Issue *create_issue() { return new ers::InvalidReferenceIssue(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::InvalidReferenceIssue::CLASS,create_issue) ;
} // anonymous context

ers::InvalidReferenceIssue::InvalidReferenceIssue() : Issue() {} 

ers::InvalidReferenceIssue::InvalidReferenceIssue(const Context &context, ers_severity s) : Issue(context,s) {} 

ers::InvalidReferenceIssue::InvalidReferenceIssue(const Context &context, ers_severity s, const void* p, const std::string &message, ers_responsibility r) : Issue(context,s) {
    severity(s);
    pointer(p); 
    finish_setup(message); 
} // InvalidReferenceIssue


const char* ers::InvalidReferenceIssue::get_class_name() const throw() {
    return ers::InvalidReferenceIssue::CLASS ; 
} // get_class_name


void ers::InvalidReferenceIssue::pointer(const void *p) {
    char buffer[256] ; 
    snprintf(buffer,sizeof(buffer),"%p",p); 
    m_value_table[REFERENCE_VALUE_KEY] = buffer ; 
    m_value_table[REFERENCE_TYPE_KEY] = REFERENCE_C_PTR_TYPE ; 
} // pointer

void ers::InvalidReferenceIssue::check(const Context &context, ers_severity s, const void* p, const char* ptr_name, ers_responsibility r) {
    if (p==0) {
	std::ostringstream message ;
	message << "pointer '" << ptr_name << "' is null" ; 
	ers::InvalidReferenceIssue issue(context,s,p,message.str(),r) ;
	issue.m_value_table[REFERENCE_NAME_KEY] = ptr_name ; 
	throw issue ; 
    } // if
} // check
    



