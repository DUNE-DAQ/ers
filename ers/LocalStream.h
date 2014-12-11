#ifndef ERS_LOCAL_STREAM_H
#define ERS_LOCAL_STREAM_H

/*
 *  LocalStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 21.11.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file LocalStream.h This file defines ERS LocalStream class.
  * \author Serguei Kolos
  * \brief ers header and documentation file
  */

#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition.hpp>

#include <ers/Issue.h>
#include <ers/IssueCatcherHandler.h>

ERS_DECLARE_ISSUE(  ers,					// namespace
		    IssueCatcherAlreadySet, 			// issue class name
		    "Local error catcher has been already set",	//
		     ERS_EMPTY					// no attributes
		 )
                     
namespace ers
{    
    class Issue;
    template <class > class SingletonCreator;
    
    class IssueCatcherHandler;
    
    /** The \c LocalStream class implements error reporting between different threads within one application.
      *
      * \author Serguei Kolos
      * \version 1.2
      * \brief Manager of ERS streams configuration.
      * \see ers::Configuration
      */

    class LocalStream
    {
        friend class IssueCatcherHandler;
	template <class > friend class SingletonCreator;
        
      public:
	        
        //! returns the singleton
        static LocalStream & instance();

	//! sets local issue intercepter
	IssueCatcherHandler * set_issue_catcher( 
        			const boost::function<void ( const ers::Issue & )> & catcher )
			throw ( ers::IssueCatcherAlreadySet );

	void error( const ers::Issue & issue );
	
        void fatal( const ers::Issue & issue );
	
        void warning( const ers::Issue & issue );

      private:
	LocalStream( );
	~LocalStream( );
        
        void remove_issue_catcher();

	void report_issue( ers::severity type, const ers::Issue & issue );
        
	void thread_wrapper();

      private:
	boost::function<void ( const ers::Issue & )>	m_issue_catcher;
	std::unique_ptr<boost::thread>			m_issue_catcher_thread;
	boost::mutex					m_mutex;
	boost::condition				m_condition;
	bool						m_terminated;
	std::queue<ers::Issue *>			m_issues;
	pthread_t					m_catcher_thread_id;
    };
}

#endif

