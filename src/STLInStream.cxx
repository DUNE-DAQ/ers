/*
 *  STLInStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 09.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/STLInStream.h"
#include "ers/Issue.h"
#include "ers/IssueFactory.h"

ers::Issue *ers::STLInStream::receive() {
    string_map_type values ;
    bool status=read_properties(values);
    if (status) {
	std::string class_name = values[Issue::ISSUE_CLASS_NAME] ; 
	return ers::IssueFactory::instance()->build(class_name); 
    } else {
	return 0 ; 
    } // if read worked
} // receive
