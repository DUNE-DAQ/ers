/*
 *  NotImplemented.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/NotImplemented.h"
#include <iostream>
#include <sstream>

const char *ers::NotImplemented::NOTIMPLEMENTED_CLASS = "ers::NotImplemented" ; 

namespace {
    ers::Issue *create_issue() { return new ers::NotImplemented(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::NotImplemented::NOTIMPLEMENTED_CLASS,create_issue) ;
} 

ers::NotImplemented::NotImplemented() : Assertion() {} 

ers::NotImplemented::NotImplemented(const Context &c, ers_severity s) : Assertion(c,s) {
    this->setup(c.function(),"",true);  
} // NotImplemented
 
/** Builds the message for a the not implemented issue.
  * @param condition_text should contain the problem condition, i.e the part that is not implemented,
  *  this should contain the function name or some similar information
  * @param message an optional message (this is displayed between parentheses). 
  * @param constant_expression is the condition constant, this should be the case for not implemented functions.
  * @return A complete information message. 
  */

std::string ers::NotImplemented::build_message(const char* condition_text, const std::string &message, bool constant_expression) {
    std::ostringstream message_stream ;
    message_stream << "Function in " << condition_text ;
    if (message!="") {
	message_stream << "(" << message << ")" ; 
    }
    message_stream << " is not implemented" ;
    return message_stream.str(); 
} // build_message


const char* ers::NotImplemented::get_class_name() const throw () { return NOTIMPLEMENTED_CLASS ;} 
