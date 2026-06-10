/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82 (2020-04-14).
 * Renamed since fork: yes (from ers/IssueReceiver.h to include/ers/IssueReceiver.hpp).
 */

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

#include <ers/Issue.hpp>

/** \file IssueReceiver.h Defines abstract interface for ERS input streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */
namespace ers
{
    class Issue; 

    /** ERS Issue receiver abstract interface.
      * User must create a subsclass of this class in order to receive issues.
      *
      * \author Serguei Kolos
      * \brief ERS Issue receiver interface.
      */
    
    class IssueReceiver
    {
      public:
	virtual ~IssueReceiver()
        { ; }
        
	virtual void receive( const Issue & issue ) = 0;	/**< \brief Is called when a new issue is received */
              
      protected:
        IssueReceiver() = default;

      private:
	IssueReceiver( const IssueReceiver & ) = delete;
        IssueReceiver & operator=( const IssueReceiver & ) = delete;
    };
}

#endif

