/*
 *  LogIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_LOG_ISSUE

#include "ers/Issue.h"

namespace ers {
    
    /** This issue is used to encapsulate simple text message like those used for logging
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief Wrapper for log messages.
      */
    
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

#define ERS_LOG_ISSUE(m,s) LogIssue(ERS_HERE,s,m) 

#endif
