/*
 *  IssueFactory.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 30.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/IssueFactory.h"
#include "ers/Issue.h"
#include "ers/Precondition.h"

ers::IssueFactory  *ers::IssueFactory::s_factory = 0 ; 

ers::IssueFactory::IssueFactory() {} ; 

/** Finds the singleton instance of the factory.
  * If the instance is not allocated yet, it is. 
  * @return a pointer to the singleton instance 
  */

ers::IssueFactory *ers::IssueFactory::instance()  {
    if (0==s_factory) {
	s_factory = new IssueFactory();
    } // creation needed
    return s_factory ; 
} // instance

/** Register an issue type with the factory 
  * @param name the name that will be used to lookup new instances 
  * @param creator a pointer to the function used to create new instance for that particular type of function
  * @return \c true if the name was already present in the table
  */

bool ers::IssueFactory::register_issue(const std::string &name, CreateIssueCallback creator) {
    return m_factory_map.insert(CallbackMap::value_type(name,creator)).second; 
} // register_Issue

/** Builds an issue out of the name it was registered with 
  * @param name the name used to indentify the class 
  * @return an newly allocated instance of this type
  */

ers::Issue *ers::IssueFactory::build(const std::string &name) const {
    CallbackMap::const_iterator i = m_factory_map.find(name); 
    if (i == m_factory_map.end()) {
	std::string message = "Cannot construct issue '"+name +"'"; 
	throw ers::Issue(ERS_HERE,ers_error,message);
    } // Not found
    ers::Issue *issue = (i->second)() ; 
    if (0==issue) {
	std::string message = "Factory for issue "+name+" returned null" ; 
	throw ers::Issue(ERS_HERE,ers_error,message);
    } // issue null 
    return issue ; 
} // build

/** Builds an issue out of a name and a value table 
  * @param name the name used to indentify the class 
  * @param values the value-table containing the state for the Issue
  * @return an newly allocated instance of this type or null if the name is not registered 
  */

ers::Issue *ers::IssueFactory::build(const std::string &name, const ers::string_map_type *values) const {
    Issue *i = build(name);
    if (i) {
	i->set_values(*values); 
    } // if i
    return i ; 
} // build

/** Clones an Issue.
  * This method creates (using the other build methods) an new Issue with the same value table
  * @param original the Issue to be cloned 
  * @return a new cloned Issued
  * @note The problem of cause exception is not handled yet. 
  */

ers::Issue *ers::IssueFactory::build(const Issue *original) {
    ERS_PRECONDITION(original!=0,"null original issue"); 
    const std::string name = original->get_class_name() ; 
    const ers::string_map_type *values = original->get_value_table();
    ERS_ASSERT(values!=0,"null value table for original"); 
    ers::Issue *clone_issue = build(name,values);
    return clone_issue ; 
}// build

