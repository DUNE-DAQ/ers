/*
 *  IOIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 14.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_POSIX_ISSUE
#define ERS_POSIX_ISSUE

#include <ios>
#include "ers/PosixIssue.h"


namespace ers {
    
    /** This class represents POSIX input/output issues.
      * \author Matthias Wiesmann
      * \version 1.0 
      */
    
    class IOIssue : public PosixIssue {
protected:
	IOIssue(const Context &c, ers_severity s, const std::exception *cause);
	IOIssue(const Context &c, ers_severity s);
public:
	static const char *IO_ISSUE_CLASS ; 
	IOIssue(); 
	IOIssue(const Context &c, ers_severity s, const std::string &message) ; 
	IOIssue(const Context &c, ers_severity s, const std::string &message, const std::exception *cause) ; 
	 
	IOIssue(const Context &c, ers_severity s, const std::string &message, const std::ios_base::failure *f); 
	virtual const char *get_class_name() const throw();
    } ; // IOIssue   
    
} // namespace ers 

#define ERS_IOERROR(msg,...) IOIssue(ERS_HERE,ers_error,msg,__VA_ARGS__) 

#endif

