/*
 *  DefaultIssue.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 14.02.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/DefaultIssue.h"

/** Constructor 
  * We simply store the name of the class that should be represented. 
  * This way the class knows what it should be.
  */

ers::DefaultIssue::DefaultIssue(const std::string &name) : Issue() {
    m_class_name = name ; 
} // DefaultIssue

ers::DefaultIssue::~DefaultIssue() throw() {} 


const char *ers::DefaultIssue::get_class_name() const throw() {
    return m_class_name.c_str(); 
} // get_class_name