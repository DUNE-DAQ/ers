/*
 *  ExampleIssue.h
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */



#include "ers/ers.h"
#include "ExampleIssue.h"



/** \file test.cxx 
  * This file contains a very simple example of using ERS. 
  * Basically, we use the custom issue defined in ExampleIssue. 
  */

/** This function illustrates the usage of range checking macros.
  * \param n an integer excepted to be between 0 and 42.
  */

void foo(int n) {
    ERS_RANGE_CHECK(0,n,42) ; 
    ERS_DEBUG_0("function foo called with value %d",n);
} // foo


int main(int, char**) {
    ers::Context::add_qualifier("ers_test") ;   // we add a qualifier to all issues 
      try { // We need to work with a try/catch block 
	ers::StreamFactory::set_stream(ers::debug_3,"filter:?!ers_test,ers_failure@default"); // we filter out all issue with qualifier ers_test at level debug_3
	ERS_DEBUG_3("This should not be displayed"); 
        ERS_DEBUG_0("checking static assert");
	ERS_STATIC_ASSERT(sizeof(int)==4);            
	ERS_DEBUG_0("dispatching custom issue to warning stream"); 
	ExampleIssue issue(ERS_HERE,ers::warning,10); // we build an instance of our Issue
	ers::StreamFactory::dispatch(issue);   // dispatch sends it to the correct stream based on severity
	ERS_DEBUG_0("calling a method with wrong range"); 
	foo(43); 
	ERS_DEBUG_0("done - if we reached this point, assertion have been disabled - this can be done by defining the N_DEBUG macro");
	ERS_DEBUG_0("throwing custom issue");
	throw ExampleIssue(ERS_HERE,ers::error,25); 
    } catch (ers::Issue &e) { // we catch issues and send them to the warning stream 
	ers::StreamFactory::warning(&e); 
    }
    return 0 ; 
} // main 

