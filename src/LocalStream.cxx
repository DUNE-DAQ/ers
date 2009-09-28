/*
 *  LocalStream.cxx
 *  ERS
 *
 *  Created by Serguei Kolos on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
#include <boost/bind.hpp>

#include <ers/LocalStream.h>
#include <ers/StreamManager.h>
#include <ers/internal/SingletonCreator.h>

namespace
{
    ers::SingletonCreator<ers::LocalStream>   _creator_;
}

/** This method returns the singleton instance.
  * It should be used for every operation on the factory.
  * \return a reference to the singleton instance
  */
ers::LocalStream &
ers::LocalStream::instance()
{
    /**Singleton instance
      */
    static ers::LocalStream * instance = new ers::LocalStream;

    _creator_.dummy();

    return *instance;
}

/** Private constructor - can not be called by user code, use the \c instance() method instead
  * \see instance()
  */
ers::LocalStream::LocalStream( )
  : m_terminated( false ),
    m_catcher_thread_id( 0 )
{ }

ers::LocalStream::~LocalStream( )
{
    {
	m_terminated = true;
	boost::mutex::scoped_lock lock( m_mutex );
	m_condition.notify_one();
    }
    if ( m_issue_catcher_thread.get() )
    {
    	m_issue_catcher_thread->join();
    }
}

void
ers::LocalStream::thread_wrapper()
{
    m_catcher_thread_id = pthread_self();
    while( !m_terminated )
    {
    	{
	    boost::mutex::scoped_lock lock( m_mutex );
	    m_condition.wait( lock );
        }
	boost::mutex::scoped_lock lock( m_issues_guard );
        while( !m_terminated && !m_issues.empty() )
        {
            ers::Issue * issue = m_issues.front();
            m_issues.pop();
            
            lock.unlock();            
            m_issue_catcher( *issue );
	    delete issue;
            lock.lock();            
        }
    }
    m_catcher_thread_id = 0;
}

void 
ers::LocalStream::set_issue_catcher( const boost::function<void ( const ers::Issue & )> & catcher ) 
		throw ( ers::IssueCatcherAlreadySet )
{
    if ( m_issue_catcher_thread.get() )
    {
    	throw ers::IssueCatcherAlreadySet( ERS_HERE );
    }
    m_issue_catcher = catcher;
    m_issue_catcher_thread.reset( new boost::thread( boost::bind( &ers::LocalStream::thread_wrapper, this ) ) );
}

void 
ers::LocalStream::report_issue( ers::severity type, const ers::Issue & issue )
{
    if ( m_catcher_thread_id && m_catcher_thread_id != pthread_self() )
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
    else
    {
	StreamManager::instance().report_issue( type, issue );
    }
}

void 
ers::LocalStream::error( const ers::Issue & issue )
{
    report_issue( ers::Error, issue );
}

void 
ers::LocalStream::fatal( const ers::Issue & issue )
{
    report_issue( ers::Fatal, issue );
}

void 
ers::LocalStream::warning( const ers::Issue & issue )
{
    report_issue( ers::Warning, issue );
}
