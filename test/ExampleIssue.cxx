/*
 *  ExampleIssue.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <iostream>
#include "ExampleIssue.h"

namespace {
    ers::Issue *create_example_issue() { 
	ers::Issue *i = new ExampleIssue(); 
	return i ; 
    } // create_issue
    bool registered = ers::IssueFactory::instance()->register_issue(ExampleIssue::CLASS_NAME,create_example_issue) ;
} 

const char * const ExampleIssue::CLASS_NAME = "ExampleIssue" ; 
const char * const ExampleIssue::PROCRASTINATION_LEVEL_KEY = "PROCRASTINATION_LEVEL" ; 

ExampleIssue::ExampleIssue() : ers::Issue() {} 

ExampleIssue::ExampleIssue(const ers::Context& context, ers::ers_severity severity) : ers::Issue(context,severity) {}

ExampleIssue::ExampleIssue(const ers::Context& context, ers::ers_severity severity, int procrastination_level) : ers::Issue(context,severity) {
    set_value(PROCRASTINATION_LEVEL_KEY,procrastination_level) ; 
    finish_setup("Procrastinating"); 
} // ExampleIssue

int ExampleIssue::procrastination_level() const {
    return get_int_value(PROCRASTINATION_LEVEL_KEY); 
} // procrastination_level

const char *ExampleIssue::get_class_name() const throw() {
    return ExampleIssue::CLASS_NAME ; 
} // get_class_name


