/*
 *  StreamManager.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Modified by Serguei Kolos on 12.09.06.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <assert.h>
#include <iostream>

#include <ers/Issue.hpp>
#include <ers/InputStream.hpp>
#include <ers/OutputStream.hpp>
#include <ers/StreamManager.hpp>
#include <ers/StreamFactory.hpp>
#include <ers/Severity.hpp>
#include <ers/Configuration.hpp>
#include <ers/ers.hpp>
#include <ers/internal/macro.hpp>
#include <ers/internal/Util.hpp>
#include <ers/internal/PluginManager.hpp>
#include <ers/internal/NullStream.hpp>
#include <ers/internal/SingletonCreator.hpp>

ERS_DECLARE_ISSUE(      ers,
                        BadConfiguration,
                        "The stream configuration string \"" << config << "\" has syntax errors.",
                        ((std::string)config) )

namespace
{
    /** This variable contains the default keys for building the default streams.
      * The default is to use the default stream, in verbose mode for errors and fatals.
      */
    const char SEPARATOR = ',';
    
    const char * const DefaultOutputStreams[] =
    {
	"lstdout",		// Debug
	"lstdout",		// Log
	"throttle,lstdout",	// Information
	"throttle,lstderr",	// Warning
        "throttle,lstderr",	// Error
        "lstderr"		// Fatal
    };
    
    const char *
    get_stream_description( ers::severity severity )
    {
	assert( ers::Debug <= severity && severity <= ers::Fatal );
        
	std::string env_name( "DUNEDAQ_ERS_" );
	env_name += ers::to_string( severity );
	const char * env = ::getenv( env_name.c_str() );
	return env ? env : DefaultOutputStreams[severity];
    }
    
    void
    parse_stream_definition(	const std::string & text,
				std::vector<std::string> & result )
    {
	std::string::size_type start_p = 0, end_p = 0;
	short brackets_open = 0;
	while ( end_p < text.length() )
	{
	    switch ( text[end_p] )
	    {
		case '(':
		    ++brackets_open;
		    break;
		case ')':
		    --brackets_open;
		    break;
		case SEPARATOR:
		    if ( !brackets_open )
		    {
			result.push_back( text.substr( start_p, end_p - start_p ) );
			start_p = end_p + 1;
		    }
                    break;
		default:
		    break;
	    }
	    end_p++;
	}
        if ( brackets_open )
        {
            throw ers::BadConfiguration( ERS_HERE, text );
        }
        if ( start_p != end_p )
        {
	    result.push_back( text.substr( start_p, end_p - start_p ) );
        }
    }
}

namespace ers
{
    // Performs lazy srteam initialization. Stream instances are created 
    // at the first attempt of writing to the stream
    class StreamInitializer : public ers::OutputStream
    {
	public:
	  StreamInitializer( StreamManager & manager )
            : m_manager( manager ),
              m_in_progress( false )
          { ; }
        
          void write( const Issue & issue ) 
          {
	    ers::severity s = issue.severity();
	    std::scoped_lock lock( m_mutex );

	    if ( !m_in_progress ) {
		m_in_progress = true;
            }
	    else {
		// The issue is coming from the stream constructor
                // We can't use ERS streams, so print it to std
                if ( s < ers::Warning )
                    std::cout << issue << std::endl;
                else
                    std::cerr << issue << std::endl;
                return ;
            }

	    if ( m_manager.m_out_streams[s].get() == this ) {
		m_manager.m_out_streams[s] =
		    std::shared_ptr<OutputStream>( m_manager.setup_stream( s ) );
	    }
	    m_manager.report_issue( s, issue );
            m_in_progress = false;
	  }
          
        private:
	  std::recursive_mutex   m_mutex;
	  StreamManager &	 m_manager; 
          bool			 m_in_progress;
    };
    
}

/** This method returns the singleton instance. 
  * It should be used for every operation on the factory. 
  * \return a reference to the singleton instance 
  */
ers::StreamManager &
ers::StreamManager::instance()
{
    /**Singleton instance
      */
    static ers::StreamManager * instance = ers::SingletonCreator<ers::StreamManager>::create();
    
    return *instance;
} // instance

/** Private constructor - can not be called by user code, use the \c instance() method instead
  * \see instance() 
  */
ers::StreamManager::StreamManager()
{
    for( short ss = ers::Debug; ss <= ers::Fatal; ++ss )
    {	
       m_init_streams[ss] = std::make_shared<StreamInitializer>( *this );
       m_out_streams[ss] = m_init_streams[ss];
    }
}

/** Destructor - basic cleanup
  */
ers::StreamManager::~StreamManager()
{ ; }

void
ers::StreamManager::add_output_stream( ers::severity severity, ers::OutputStream * new_stream )
{    
    std::shared_ptr<OutputStream> head = m_out_streams[severity];
    if ( head && !head->isNull() )
    {
	OutputStream * parent = head.get();
        for ( OutputStream * stream = parent; !stream->isNull(); parent = stream, 
        	stream = &parent->chained() )
            ;
                 
	parent->chained( new_stream );
    }
    else
    {
        m_out_streams[severity] = std::shared_ptr<OutputStream>( new_stream );
    }
}	

void
ers::StreamManager::add_receiver( const std::string & stream,
				  const std::string & filter,
                                  ers::IssueReceiver * receiver )
{
    InputStream * in = ers::StreamFactory::instance().create_in_stream( stream, filter );
    in->set_receiver( receiver );
    
    std::scoped_lock lock( m_mutex );
    m_in_streams.push_back( std::shared_ptr<InputStream>( in ) );
}

void 
ers::StreamManager::add_receiver( const std::string & stream,
				  const std::initializer_list<std::string> & params,
				  ers::IssueReceiver * receiver )
{
    InputStream * in = ers::StreamFactory::instance().create_in_stream( stream, params );
    in->set_receiver( receiver );
    
    std::scoped_lock lock( m_mutex );
    m_in_streams.push_back( std::shared_ptr<InputStream>( in ) );
}

void
ers::StreamManager::remove_receiver( ers::IssueReceiver * receiver )
{
    std::scoped_lock lock( m_mutex );
    for( std::list<std::shared_ptr<InputStream> >::iterator it = m_in_streams.begin();
    	it != m_in_streams.end(); )
    {	
        if ( (*it) -> m_receiver == receiver )
            m_in_streams.erase( it++ );
        else
            ++it;
    }
}

ers::OutputStream * 
ers::StreamManager::setup_stream( ers::severity severity )
{    
    std::string config = get_stream_description( severity );
    std::vector<std::string> streams;
    try
    {
    	parse_stream_definition( config, streams );
    }
    catch ( ers::BadConfiguration & ex )
    {
	ERS_INTERNAL_ERROR(	"Configuration for the \"" << severity << "\" stream is invalid. "
        			"Default configuration will be used." );
    }

    ers::OutputStream * main = setup_stream( streams );
    
    if ( !main )
    {
	std::vector<std::string> default_streams;
	try
	{
	    parse_stream_definition( DefaultOutputStreams[severity], default_streams );
	    main = setup_stream( default_streams );
        }
	catch ( ers::BadConfiguration & ex )
	{
	    ERS_INTERNAL_ERROR( "Can not configure the \"" << severity 
            	<< "\" stream because of the following issue {" << ex << "}" );
	}
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

/** Sends an Issue to an appropriate stream 
 * \param type 
 * \param issue 
 */
void
ers::StreamManager::report_issue( ers::severity type, const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( type );
    m_out_streams[type]->write( issue );
    issue.set_severity( old_severity );
} // error

/** Sends an Issue to the error stream 
 * \param issue 
 */
void
ers::StreamManager::error( const Issue & issue )
{
    report_issue( ers::Error, issue );
} // error

/** Sends an issue to the debug stream 
 * \param issue the Issue to send
 * \param level the debug level. 
 */
void
ers::StreamManager::debug( const Issue & issue, int level )
{
    if ( Configuration::instance().debug_level() >= level )
    {
	ers::severity old_severity = issue.set_severity( ers::Severity( ers::Debug, level ) );
	m_out_streams[ers::Debug]->write( issue );
	issue.set_severity( old_severity );
    }
}

/** Sends an Issue to the fatal error stream 
 * \param issue 
 */
void
ers::StreamManager::fatal( const Issue & issue )
{
    report_issue( ers::Fatal, issue );
}

/** Sends an Issue to the warning stream 
* \param issue the issue to send
*/
void
ers::StreamManager::warning( const Issue & issue )
{
    report_issue( ers::Warning, issue );
}

/** Sends an issue to the info stream 
 * \param issue the Issue to send
 */
void
ers::StreamManager::information( const Issue & issue )
{
    report_issue( ers::Information, issue );
}

/** Sends an issue to the log stream 
 * \param issue the Issue to send
 */
void
ers::StreamManager::log( const Issue & issue )
{
    report_issue( ers::Log, issue );
}

std::ostream & 
ers::operator<<( std::ostream & out, const ers::StreamManager & )
{
    for( short ss = ers::Debug; ss <= ers::Fatal; ++ss )
    {	
	out << (ers::severity)ss << "\t\"" 
        	<< get_stream_description( (ers::severity)ss ) << "\"" << std::endl;
    }
    return out;
}
