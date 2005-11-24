/*
 *  IssueReceiver.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_ISSUE_RECEIVER_H
#define ERS_ISSUE_RECEIVER_H

#include <ers/Issue.h>

/** \file IssueReceiver.h Defines abstract interface for ERS input streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

namespace ers
{
    
    class Issue; 

    /** ERS Issue receiver interface.
      * User must create a subsclass of this class in order to receive issues.
      * \author Serguei Kolos
      * \version 1.0
      * \brief ERS Issue receiver interface.
      */
    
    class IssueReceiver
    {
      public:
	virtual ~IssueReceiver()
        { ; }
        
	virtual void receive( const Issue & issue ) = 0;	/**< \brief Is called when the new issue is received */
              
      protected:
        IssueReceiver()
        { ; }
                                       
      private:
	//
	// Disable copying
	//
	IssueReceiver( const IssueReceiver & );
        IssueReceiver & operator=( const IssueReceiver & );
    };
}

#endif

