/*
 *  ParseIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 17.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_PARSE_ISSUE
#define ERS_PARSE_ISSUE

#include "ers/Issue.h"

namespace ers {
    
    /** This class represents an error while parsing data. 
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
class ParseIssue: public Issue {
protected:
    ParseIssue(const Context &c, ers_severity s) ; 
public:
    static const char *OFFENDING_LINE_KEY ;
    static const char *PARSE_ISSUE_CLASS_NAME ; 
    void offending_line(const std::string &line) ; 
    ParseIssue() ; 
    ParseIssue(const Context &c,ers_severity s, const std::string &message); 
    ParseIssue(const Context &c,ers_severity s, const std::string &message, const std::string &line); 
    virtual const char* get_class_name() const throw() ;
} ; // ParseIssue

} // ers

#define ERS_PARSE_ERROR(message,...) ParseIssue(ERS_HERE,ers::ers_error,__VA_ARGS__) 

#endif