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

ers::IssueFactory  *ers::IssueFactory::s_factory = 0 ; 

ers::IssueFactory::IssueFactory() {} ; 

ers::IssueFactory *ers::IssueFactory::instance()  {
    if (0==s_factory) {
	s_factory = new IssueFactory();
    } // creation needed
    return s_factory ; 
} // instance

bool ers::IssueFactory::register_issue(const std::string &name, CreateIssueCallback creator) {
    return factory_map.insert(CallbackMap::value_type(name,creator)).second; 
} // register_Issue

ers::Issue *ers::IssueFactory::build(const std::string &name) const {
    CallbackMap::const_iterator i = factory_map.find(name); 
    if (i == factory_map.end()) {
	std::string message = "Cannot construct issue "+name ; 
	throw ers::Issue(message,ers_error,ERS_HERE);
    } // Not found
    ers::Issue *issue = (i->second)() ; 
    if (0==issue) {
	std::string message = "Factory for issue "+name+" returned null" ; 
	throw ers::Issue(message,ers_error,ERS_HERE);
    } // issue null 
    return issue ; 
} // build

ers::Issue *ers::IssueFactory::build(const std::string &name, const ers::string_map_type *values) const {
    Issue *i = build(name);
    if (i) {
	i->set_values(*values); 
    } // if i
    return i ; 
} // build

ers::Issue *ers::IssueFactory::build(const Issue *original) {
    const std::string name = original->get_class_name() ; 
    const ers::string_map_type *values = original->get_value_table(); 
    ers::Issue *clone_issue = build(name,values);
    return clone_issue ; 
}// build

