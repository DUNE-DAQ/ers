/*
 *  IssueFactory.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 30.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_ISSUE_FACTORY
#define ERS_ISSUE_FACTORY

#include <string>
#include <map>
#include "ers/util.h"

namespace ers {

class Issue ; 
    
    /** This class implements the factory pattern for Issues.
      * The main responsability of this class is to keep track of the existing types of Issues
      * Each issue should register one factory method for instances of this class.
      * This is needed for deserilizing of Issues. 
      * @author Matthias Wiesmann
      * @version 1.0
      */
    
class IssueFactory {
public:
    typedef Issue* (*CreateIssueCallback)();
    typedef std::map<std::string,CreateIssueCallback> CallbackMap ; 
protected:
    IssueFactory(); 
    CallbackMap m_factory_map ; 
    static IssueFactory  *s_factory ; 
public:
    static IssueFactory *instance() ;
    bool register_issue(const std::string &name, CreateIssueCallback creator) ;
    Issue *build(const std::string &name) const ; 
    Issue *build(const std::string &name, const string_map_type *values) const ; 
    Issue *build(const Issue *original); 
    } ; // IssueFactory
} // ers

#endif

