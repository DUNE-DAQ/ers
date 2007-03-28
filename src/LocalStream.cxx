/*
 *  LocalStream.cxx
 *  ERS
 *
 *  Created by Serguei Kolos on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
#include <boost/bind.hpp>

#include <ers/thread/LocalStream.h>
#include <ers/internal/SingletonCreator.h>

#include <ers/StreamManager.h>

namespace
{
    ers::SingletonCreator<ers::thread::LocalStream>   _creator_;
}

/** This method returns the singleton instance.
  * It should be used for every operation on the factory.
  * \return a pointer to the singleton instance
  */
ers::thread::LocalStream &
ers::thread::LocalStream::instance()
{
    /**Singleton instance
      */
    static ers::thread::LocalStream * instance = new ers::thread::LocalStream;

    _creator_.dummy();

    return *instance;
}

/** Private constructor - can not be called by user code, use the \c instance() method instead
  * \see instance()
  */
ers::thread::LocalStream::LocalStream( )
  : m_terminated( false )
{ }

ers::thread::LocalStream::~LocalStream( )
{
    m_terminated = true;
    if ( m_issue_catcher_thread.get() )
    {
    	m_issue_catcher_thread->join();
    }
}

void
ers::thread::LocalStream::thread_wrapper()
{
    while( !m_terminated )
    {
    	boost::mutex::scoped_lock lock( m_mutex );
        m_condition.wait( lock );
        while( !m_issues.empty() )
        {
            m_issue_catcher( *m_issues.front() );
            m_issues.pop();
        }
    }
}

void 
ers::thread::LocalStream::set_issue_catcher( const boost::function<void ( const ers::Issue & )> & catcher ) 
		throw ( ers::thread::IssueCatcherAlreadySet )
{
    if ( m_issue_catcher_thread.get() )
    {
    	throw ers::thread::IssueCatcherAlreadySet( ERS_HERE );
    }
    m_issue_catcher = catcher;
    m_issue_catcher_thread.reset( new boost::thread( boost::bind( &ers::thread::LocalStream::thread_wrapper, this ) ) );
}

void 
ers::thread::LocalStream::report_issue( ers::severity type, const ers::Issue & issue )
{
    if ( !m_issue_catcher_thread.get() )
    {
	StreamManager::instance().report_issue( type, issue );
    }
    else
    {
	ers::Issue * clone = issue.clone();
	clone->set_severity( type );
	{
	    boost::mutex::scoped_lock lock( m_issues_guard );
	    m_issues.push( clone );
	}
	boost::mutex::scoped_lock lock( m_mutex );
	m_condition.notify_one();
    }
}

void 
ers::thread::LocalStream::error( const ers::Issue & issue )
{
    report_issue( ers::Error, issue );
}

void 
ers::thread::LocalStream::fatal( const ers::Issue & issue )
{
    report_issue( ers::Fatal, issue );
}

void 
ers::thread::LocalStream::warning( const ers::Issue & issue )
{
    report_issue( ers::Warning, issue );
}
