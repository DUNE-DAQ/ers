/*
 *  IssueFactoryIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/IssueFactoryIssue.h"
#include <sstream>


const char* ers::IssueFactoryIssue::CLASS_NAME = "ers::IssueFactoryIssue" ; 

namespace {
    ers::Issue *create_issue() { return new ers::IssueFactoryIssue(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::IssueFactoryIssue::CLASS_NAME,create_issue) ;
} 

/** \overload 
  */

ers::IssueFactoryIssue::IssueFactoryIssue() : Issue() {} 

/** \overload
  */

ers::IssueFactoryIssue::IssueFactoryIssue(const Context &context, ers_severity s): Issue(context,s) {}

/** Constructs an Issue factory Issue
  * \param context the context of the issue, use \c MRS_HERE 
  * \param s the severity of the issue
  * \param name the name of the class requested to the factory 
  * \param message the error message associated with the issue 
  */


ers::IssueFactoryIssue::IssueFactoryIssue(const Context &context, ers_severity s, const std::string&name, const std::string &message) : Issue(context,s) {
    std::ostringstream m ;
    m << "Unable to build Issue for name '" << name << "': " << message ;
    finish_setup(m.str());
} // IssueFactoryIssue


const char *ers::IssueFactoryIssue::get_class_name() const throw() { return CLASS_NAME ; }