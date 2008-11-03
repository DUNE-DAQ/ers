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

ERS_DECLARE_ISSUE(	ers, // namespace
			IssueCatcherAlreadySet, // issue class name
			"Local error catcher has been already set", //
			 // no attributes
		 )
                     
namespace ers
{
    
    class Issue;
    
    /** The \c LocalStream class implements error reporting between different threads within one application.
      *
      * \author Serguei Kolos
      * \version 1.2
      * \brief Manager of ERS streams configuration.
      * \see ers::Configuration
      */

    class LocalStream
    {
      public:
	static LocalStream & instance();	/**< \brief return the singleton */

	void set_issue_catcher( const boost::function<void ( const ers::Issue & )> & catcher )
					throw ( ers::IssueCatcherAlreadySet ); /**< \brief can be used to set the local error handling function */

	void error( const ers::Issue & issue );
	void fatal( const ers::Issue & issue );
	void warning( const ers::Issue & issue );

      private:
	LocalStream( );
	~LocalStream( );

	void report_issue( ers::severity type, const ers::Issue & issue );
	void thread_wrapper();

	boost::function<void ( const ers::Issue & )>	m_issue_catcher;
	std::auto_ptr<boost::thread>			m_issue_catcher_thread;
	boost::mutex					m_mutex;
	boost::condition				m_condition;
	bool						m_terminated;
	boost::mutex					m_issues_guard;
	std::queue<ers::Issue *>			m_issues;
	pthread_t					m_catcher_thread_id;
    };
}

#endif

