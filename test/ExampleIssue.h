/*
 *  ExampleIssue.h
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/Issue.h"

class ExampleIssue : public ers::Issue {
    
protected:
    ExampleIssue(const ers::Context& context, ers::ers_severity severity) ;
public:
    static const char* CLASS_NAME ; 
    static const char* PROCRASTINATION_LEVEL_KEY ; 
    ExampleIssue();
    ExampleIssue(const ers::Context& context, ers::ers_severity severity, int procrastination_level) ;
    virtual const char *get_class_name() const throw() ;
    int procrastination_level() const ; 
} ; // ExempleIssue

#define EXAMPLE_ERROR(level) ExampleIssue(ERS_HERE,ers_error,level)


