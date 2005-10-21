/*
 *  StreamFactory.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <iostream>

#include <ers/Issue.h>
#include <ers/Stream.h>
#include <ers/StreamFactory.h>
#include <ers/internal/NullStream.h>
#include <ers/Severity.h>
#include <ers/internal/Util.h>
#include <ers/ers.h>

namespace
{
    /** This variable contains the default keys for building the default streams.
      * The default is to use the default stream, in verbose mode for errors and fatals.
      */
    const char * const SEPARATORS = ",";
    
    const char * DefaultConfiguration[] =
    {
	"stdout",		// Debug
	"stdout",		// Information
	"stderr",		// Warning
        "stderr",		// Error
        "stderr,abort"		// Fatal
    };
    
    void
    read_environment( const char * name, int & value )
    {
	const char * env = ::getenv( name );
	if ( env )
	{
	    if ( sscanf( env, "%d", &value ) != 1 )
	    {
		ERS_WARNING( "Wrong value \"" << env << "\" is given for the \"" << name << "\" environment" )
	    }
	}
    }
    
    const char *
    get_stream_description( ers::severity severity )
    {
	ERS_RANGE_CHECK( ers::Debug, severity, ers::Fatal )
        
	std::string env_name( "TDAQ_ERS_" );
	env_name += ers::to_string( severity );
	const char * env = ::getenv( env_name.c_str() );
	return env ? env : DefaultConfiguration[severity];
    }
}

/** Private constructor - can not be called by user code, use the \c instance() method instead
  * \see instance() 
  */
ers::StreamFactory::StreamFactory()
  : m_debug_level( 0 ),
    m_verbosity_level( 0 )
{
    for( ers::severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {	
       m_streams[ss] = 0;
    }
    m_null_stream = new ers::NullStream();
    
    read_environment( "TDAQ_ERS_DEBUG_LEVEL", m_debug_level );
    read_environment( "TDAQ_ERS_VERBOSITY_LEVEL", m_verbosity_level );
} // StreamFactory

/** Destructor - basic cleanup
  * \note in practice this method should never be called, the singleton should never be deallocated
  */
ers::StreamFactory::~StreamFactory()
{
    for( ers::severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {	
       delete m_streams[ss];
    }
    delete m_null_stream;
} // ~StreamFactory


/** This method returns the singleton instance. 
  * It should be used for every operation on the factory. 
  * \return a pointer to the singleton instance 
  */
ers::StreamFactory &
ers::StreamFactory::instance()
{
    /**Singleton instance
      */
    static ers::StreamFactory instance;
    return instance;
} // instance


ers::Stream * 
ers::StreamFactory::get_stream( ers::severity severity )
{
    ERS_RANGE_CHECK( ers::Debug, severity, ers::Fatal )
    
    if ( !m_streams[severity] )
    {
    	std::string config = get_stream_description( severity );
        std::vector<std::string> streams;
    	ers::tokenize( config, SEPARATORS, streams );
        
	ers::Stream * main = m_streams[severity] = create_stream( streams[0], DefaultConfiguration[severity] );
	for ( size_t i = 1; i < streams.size(); i++ )
	{
	   ers::Stream * chained = create_stream( streams[i], DefaultConfiguration[severity] );
	   main->chained( chained );
	   main = chained;
	}
    }
        
    return m_streams[severity];
}

/** Builds a stream from a textual key 
  * The key should have the format \c protocol:path.extension
  * In certain cases, the path will be empty. 
  * For instance to write in XML format to the error stream, the key is: 
  * \c cerr:.xml 
  * \param format the format, which describes new stream
  * \param default_key the key, which will be used for the new stream creation if the format could ot be parsed successfully
  * \note the stream is allocated on the heap, it is the caller's responsibility to delete it.
  */
ers::Stream *
ers::StreamFactory::create_stream(	const std::string & format,
					const std::string & default_key ) const
{
    std::string key = format;
    std::string param;
    std::string::size_type start = format.find( '(' );    
    if ( start != std::string::npos )
    {
	key = format.substr( 0, start );
	std::string::size_type end = format.find( ')', start );
        if ( end != std::string::npos )
            param = format.substr( start + 1, end - start - 1 );
    }    	

    FunctionMap::const_iterator it = m_factories.find( key );
    
    if( it != m_factories.end() )
    {
	return it->second( param );
    }
    
    ERS_WARNING( "Can not find stream creator for \"" << key << "\" stream" )
    it = m_factories.find( default_key );
    if( it != m_factories.end() )
    {
	return it->second( std::string() );
    }
    
    return new ers::NullStream(); 
}

/** Sends an Issue to the error stream 
 * \param issue_ptr 
 */
void
ers::StreamFactory::error( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Error );
    get_stream(ers::Error)->write( issue );
    issue.set_severity( old_severity );
} // error

ers::Stream *
ers::StreamFactory::debug( int level )
{
    return ( m_debug_level >= level ? get_stream( ers::Debug ) : m_null_stream );
}

/** Sends an issue to the debug stream 
 * \param issue_ptr the Issue to send
 * \param level the debug level. 
 */
void
ers::StreamFactory::debug( const Issue & issue, int level )
{
    ers::Severity old_severity = issue.set_severity( ers::Severity( ers::Debug, level ) );
    debug( level )->write( issue );
    issue.set_severity( old_severity );
} //  debug

/** Sends an Issue to the fatal error stream 
 * \param issue_ptr 
 */
void
ers::StreamFactory::fatal( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Fatal );
    get_stream( ers::Fatal )->write( issue );
    issue.set_severity( old_severity );
} // fatal

/** Sends an Issue to the warning stream 
* \param issue_ptr the issue to send
*/
void
ers::StreamFactory::warning( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Warning );
    get_stream( ers::Warning )->write( issue );
    issue.set_severity( old_severity );
} // warning

/** Sends an issue to the debug stream 
 * \param issue_ptr the Issue to send
 * \param level the debug level. 
 */
void
ers::StreamFactory::information( const Issue & issue )
{
    ers::severity old_severity = issue.set_severity( ers::Information );
    get_stream( ers::Information )->write( issue );
    issue.set_severity( old_severity );
} //  debug

/** Registers a factory function with the stream factory.
  * The callback is function that takes two parameters <ol>
  * <li>a string describing the protocol, for instance \c file </li>
  * <li>a string describing a uri, can be a path, a suffix or anything</li>
  * </ol>
  * The function should return a heap allocated stream, or null if it does not
  * understand the protocol. 
  * \param name name of the stream type (human display only).
  * \param callback the callback function
  */
void
ers::StreamFactory::register_stream( const std::string & name, StreamCreator callback )
{
    m_factories[name] = callback;
} // register_stream

std::ostream & 
ers::operator<<( std::ostream & out, const ers::StreamFactory & )
{
    for( ers::severity ss = ers::Debug; ss <= ers::Fatal; ++((size_t&)ss) )
    {	
	out << ss << "\t\"" << get_stream_description( ss ) << "\"" << std::endl;
    }
    return out;
}
