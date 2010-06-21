/*
 *  QAnyIssue.h
 *  ers
 *
 *  Created by Serguei Kolos on 26.06.10.
 *  Copyright 2010 CERN. All rights reserved.
 *
 */

#ifndef ERS_ANY_ISSUE_H
#define ERS_ANY_ISSUE_H

/** \file AnyIssue.h
  * This file defines the ers::AnyIssue class
  */ 

#include <ers/Issue.h>

namespace ers
{   
    class AnyIssue : public ers::Issue
    {
      public:
        AnyIssue( const std::string & type,
                  const ers::Context & context,
		  const std::string & message = "" )
          : ers::Issue( context, message ),
            m_type( type )
        { ; }
        
        AnyIssue( const std::string & type,
                  const ers::Context & context,
		  const std::string & message,
                  const ers::Issue & cause )
          : ers::Issue( context, message, cause ),
            m_type( type )
        { ; }
        
        ~AnyIssue() throw() { ; }
        
        virtual ers::Issue * clone() const
        { return new AnyIssue( *this ); }
	
        virtual const char * get_class_name() const
        { return m_type.c_str(); }
       	
        virtual void raise() const throw( std::exception )
        { throw *this; }
        
      private:
      	std::string m_type;
    };
}

#endif

