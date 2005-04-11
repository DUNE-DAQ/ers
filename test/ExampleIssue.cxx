/*
 *  ExampleIssue.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>
#include "ExampleIssue.h"

namespace {
    /// This block of code is only needed if issues of this type will need to be deserialised outside
    /// of the process where it is first created. For instance if the Issue is created in one process,
    /// streamed to a file and read again from this file. 
    /// If the following function is not defined and registered, when an Issue is deserialised, it will 
    /// have a special type, \c DefaultIssue all the information inside the issue will be intact, but custom
    /// method will not be available. 
    /// \return 
    
    ers::Issue *create_example_issue() { 
	ers::Issue *i = new ExampleIssue(); 
	return i ; 
    } // create_issue
    
    /// We register the factory function with the IssueFactory singleton, this should only be done if 
    /// a create_issue function is defined for the Issue. 

    bool registered = ers::IssueFactory::instance()->register_issue(ExampleIssue::CLASS_NAME,create_example_issue) ;
} // anonymous namespace


const char * const ExampleIssue::CLASS_NAME = "ExampleIssue" ; 
const char * const ExampleIssue::PROCRASTINATION_LEVEL_KEY = "PROCRASTINATION_LEVEL" ; 

ExampleIssue::ExampleIssue() : ers::Issue() {} 

ExampleIssue::ExampleIssue(const ers::Context& context, ers::severity_t severity) : ers::Issue(context,severity) {}

/** User constructor, this constructor should be used by the user.
  * The prototype includes the two mandatory parameters, context and severity 
  * plus whatever custome construction parameters are needed.
  * \param context object representing the code-context where the issue originated, normally, this is the macro ERS_HERE
  * \param severity the severity of the issue 
  * \param procrastination_level this is a custom parameter needed by this example issue 
  */

ExampleIssue::ExampleIssue(const ers::Context& context, ers::severity_t severity, int procrastination_level) : ers::Issue(context,severity) {
    set_value(PROCRASTINATION_LEVEL_KEY,procrastination_level) ; 
    std::ostringstream msg_str ; 
    msg_str << "Procrastinating at level " << procrastination_level ; 
    finish_setup(msg_str.str()); 
} // ExampleIssue

/** This method is an example of an accessor for a custom property in the issue
  * It basically searches the value table for the right property and returns it.
  * \return the value of the procrastination property
  */

int ExampleIssue::procrastination_level() const {
    return get_int_value(PROCRASTINATION_LEVEL_KEY); 
} // procrastination_level

/// This function returns the name of the Issue, this name should be the same as 
/// the unmangled class name. Here we given back the string defined in ExampleIssue::CLASS_NAME
/// \note If this function is not defined, the implementation 
/// the Issue root object tries to build a meaningfull name out of RTTI information, 
/// but depending on the compiler this is unreliable. 
/// \return name of the class 

const char *ExampleIssue::get_class_name() const throw() {
    return ExampleIssue::CLASS_NAME ; 
} // get_class_name

