/*
 *  PosixIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include "ers/Issue.h"

#ifndef __POSIX_ISSUE__
#define __POSIX_ISSUE__

namespace ers {

/** This class is the root class for Posix issues 
  * It knows how to build meaningfull messages out of errno
  * @author Matthias Wiesmann
  * @version 0.1 (test). 
  */

class PosixIssue : public Issue {
protected:
    static const char* UNKNOWN_ERR_MSG ; 
    static const char* TO_LONG_ERR_MSG ; 
    static const char* UNKNOWN_ERR ; 
    static std::string error_message(int err);  
    void setup_posix(int err); 
    PosixIssue(ers_severity s, const Context &context); 
    PosixIssue(const std::exception *cause, ers_severity s, const Context &context);
public:
    static const char *POSIX_ERROR_MSG_KEY ;
    static const char *POSIX_ERROR_NBR_KEY ;
    static const char *POSIX_ERROR_CLASS ; 
    PosixIssue(); 
    PosixIssue(const std::string &message, ers_severity s, int err, const Context &context) ; 
    PosixIssue(const std::string &message, ers_severity s, const Context &context);
    PosixIssue(const std::string &message, const std::exception *cause, ers_severity s, const Context &context);
    virtual const char*get_class_name() const throw() ;
    std::string posix_message() const throw() ; 
    ~PosixIssue() throw() {} ;    
} ; // posix_issue

}

#endif

