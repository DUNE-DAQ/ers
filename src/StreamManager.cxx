/*
 *  StreamManager.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <iostream>

#include <ers/Issue.h>
#include <ers/InputStream.h>
#include <ers/OutputStream.h>
#include <ers/StreamManager.h>
#include <ers/StreamFactory.h>
#include <ers/Severity.h>
#include <ers/Configuration.h>
#include <ers/ers.h>
#include <ers/internal/Util.h>
#include <ers/internal/PluginManager.h>
#include <ers/internal/NullStream.h>

namespace
{
    /** This variable contains the default keys for building the default streams.
      * The default is to use the default stream, in verbose mode for errors and fatals.
      */
    const char * const SEPARATORS = ",";
    
    const char * const DefaultOutputStreams[] =
    {
	"stdout",		// Debug
	"stdout",		// Information
	"stderr",		// Warning
        "stderr",		// Error
        "stderr,abort"		// Fatal
    };
    
    const char *
    get_stream_description( ers::severity severity )
    {
	ERS_RANGE_CHECK( ers::Debug, severity, ers::Fatal )
        
	std::string env_name( "TDAQ_ERS_" );
	env_name += ers::to_string( severity );
	const char * env = ::getenv( env_name.c_str() );
	return env ? env : DefaultOutputStreams[severity];
    }
}

/** This method returns the singleton instance. 
  * It should be used for every operation on the factory. 
  * \return a pointer to the singleton instance 
  */
ers::StreamManager &
ers::StreamManager::instance()
{
    /**Singleton instance
      */
    static ers::PluginManager plugins;
    static ers::StreamManager instance;
    return instance;
} // instance

/** Private constructor - can not be called by user code, use the \c instance() method instead
  * \see instance() 
  */
ers::StreamManager::StreamManager()
{
    for( ers::severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {	
       m_out_streams[ss] = setup_stream( ss );
    }
}

/** Destructor - basic cleanup
  * \note in practice this method should never be called, the singleton should never be deallocated
  */
ers::StreamManager::~StreamManager()
{
    for( ers::severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {	
       delete m_out_streams[ss];
    }
}

void
ers::StreamManager::add_receiver( const std::string & stream,
				  const std::string & filter,
                                  ers::IssueReceiver * receiver ) throw ( ers::InvalidFormat )
{
    ers::InputStream * in = ers::StreamFactory::instance().create_in_stream( stream, filter );
    in->set_receiver( receiver );
    m_in_streams.push_back(  );
}

ers::OutputStream * 
ers::StreamManager::setup_stream( ers::severity severity )
{    
    std::string config = get_stream_description( severity );
    std::vector<std::string> streams;
    ers::tokenize( config, SEPARATORS, streams );

    ers::OutputStream * main = setup_stream( streams );
    
    if ( !main )
    {
	std::vector<std::string> default_streams;
	ers::tokenize( DefaultOutputStreams[severity], SEPARATORS, default_streams );
    	main = setup_stream( default_streams );
    }   
    return ( main ? main : new ers::NullStream() );
}

ers::OutputStream * 
ers::StreamManager::setup_stream( const std::vector<std::string> & streams )
{    
    size_t cnt = 0;
    ers::OutputStream * main = 0;
    for ( ; cnt < streams.size(); ++cnt )
    {
	main = ers::StreamFactory::instance().create_out_stream( streams[cnt] );
        if ( main )
            break;
    }
    
    if ( !main )
    {
    	return 0;
    }
    
    ers::OutputStream * head = main;
    for ( ++cnt; cnt < streams.size(); ++cnt )
    {
	ers::OutputStream * chained = ers::StreamFactory::instance().create_out_stream( streams[cnt] );
       
	if ( chained )
	{
	    head->chained( chained );
	    head = chained;
        }
    }
        
    return main;
}

/** Sends an Issue to the error stream 
 * \param issue_ptr 
 */
void
ers::StreamManager::error( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Error );
    m_out_streams[ers::Error]->write( issue );
    issue.set_severity( old_severity );
} // error

/** Sends an issue to the debug stream 
 * \param issue_ptr the Issue to send
 * \param level the debug level. 
 */
void
ers::StreamManager::debug( const Issue & issue, int level )
{
    if ( Configuration::instance().debug_level() >= level )
    {
	ers::Severity old_severity = issue.set_severity( ers::Severity( ers::Debug, level ) );
	m_out_streams[ers::Debug]->write( issue );
	issue.set_severity( old_severity );
    }
}

/** Sends an Issue to the fatal error stream 
 * \param issue_ptr 
 */
void
ers::StreamManager::fatal( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Fatal );
    m_out_streams[ers::Fatal]->write( issue );
    issue.set_severity( old_severity );
}

/** Sends an Issue to the warning stream 
* \param issue_ptr the issue to send
*/
void
ers::StreamManager::warning( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Warning );
    m_out_streams[ers::Warning]->write( issue );
    issue.set_severity( old_severity );
}

/** Sends an issue to the debug stream 
 * \param issue_ptr the Issue to send
 * \param level the debug level. 
 */
void
ers::StreamManager::information( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Information );
    m_out_streams[ers::Information]->write( issue );
    issue.set_severity( old_severity );
}

std::ostream & 
ers::operator<<( std::ostream & out, const ers::StreamManager & )
{
    for( ers::severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {	
	out << ss << "\t\"" << get_stream_description( ss ) << "\"" << std::endl;
    }
    return out;
}
