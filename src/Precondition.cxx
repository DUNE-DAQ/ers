/*
 *  Precondition.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Precondition.h"
#include <iostream>
#include <sstream>

const char *ers::Precondition::PRECONDITION_CLASS = "ers::Precondition" ; 

namespace {
    ers::Issue *create_issue() { return new ers::Precondition(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::Precondition::PRECONDITION_CLASS,create_issue) ;
} // anonymous context

/** Constructor used by deserialisation
 */

ers::Precondition::Precondition() : Assertion() {} 

/** Constructor used by subclasses 
 *  @param s severity of the issue 
 *  @param constext of the issue 
 */

ers::Precondition::Precondition(ers_severity s, const Context &context) : Assertion(s,context) {}  

/** Main constructor, it should be used for building Precondition Issues
  * @param condition the condition 
  */

ers::Precondition::Precondition(const char*condition, const std::string &message, ers_severity s, const Context &context, bool constant_expression) : Assertion(s,context) {
    this->setup(condition,message,constant_expression); 
} // Precondition

const char* ers::Precondition::get_class_name() const throw () { return PRECONDITION_CLASS ;} 

/** Builds the message associated with an assertion 
 * @return message describing the failure of the assertion 
 * @param condition_text the condition in textual form 
 * @param message the message associated with the condition
 * @return an human readable message 
 */

std::string ers::Precondition::build_message(const char* condition_text, const std::string &message, bool constant_expression) {
    std::ostringstream m ;
    m << "Precondition '" << condition_text << "' failed: " << message ;
    if (constant_expression) {
	m << " (this condition is constant)" ; 
    } // constant expression 
    return m.str();
} // build_message
