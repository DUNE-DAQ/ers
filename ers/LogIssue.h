/*
 *  LogIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/Issue.h"

namespace ers {
    class LogIssue : public Issue {
protected:
	LogIssue(const ers::Context &c, ers_severity s); 
public:
	static const char *CLASS_NAME ;
	LogIssue(); 
	LogIssue(const ers::Context &c, ers_severity s, const std::string &message); 
	virtual const char* get_class_name() const throw() ;

    } ; // LogIssue
} // ers
