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

const char * const ers::Precondition::PRECONDITION_CLASS = "ers::Precondition" ; 

namespace {
    ers::Issue *create_issue() { return new ers::Precondition(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::Precondition::PRECONDITION_CLASS,create_issue) ;
} // anonymous context

/** Constructor used by deserialisation
 */

ers::Precondition::Precondition() : Assertion() {} 

/** Constructor used by subclasses 
 *  \param c context of the issue
 *  \param s severity of the issue 
 *  \param c context of the issue 
 */

ers::Precondition::Precondition(const Context &c, ers_severity s) : Assertion(c,s) {}  

/** Main constructor, it should be used for building Precondition Issues
 * \param context context of the issue
 * \param s severity of the issue 
 * \param condition text of the condition of the precondition
 * \param message message for condition 
 * \param constant_expression does compiler think expression is constant
 */

ers::Precondition::Precondition(const Context &context, ers_severity s, const char*condition, const std::string &message , bool constant_expression) : Assertion(context,s) {
    this->setup(condition,message,constant_expression); 
} // Precondition

/** \return name key for the class */

const char* ers::Precondition::get_class_name() const throw () { return PRECONDITION_CLASS ;} 

/** Builds the message associated with an assertion 
 * \param condition_text the condition in textual form 
 * \param message the message associated with the condition
 * \param constant_expression is the condition constant (as detected by compiler). 
 * \return message describing the failure of the assertion 
 */

std::string ers::Precondition::build_message(const char* condition_text, const std::string &message, bool constant_expression) throw() {
    std::ostringstream m ;
    m << "Precondition '" << condition_text << "' failed: " << message ;
    if (constant_expression) {
	m << " (this condition is constant)" ; 
    } // constant expression 
    return m.str();
} // build_message
