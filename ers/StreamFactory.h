/*
 *  StreamFactory.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_STREAM_FACTORY
#define ERS_STREAM_FACTORY

#include <ers/Severity.h>
#include <ers/Context.h>

#include <map>

/** \file StreamFactory.h This file defines the StreamFactory class, 
  * which is responsible for registration and creation of ERS streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

namespace ers
{
    
    class Stream; 
    class Issue; 
    
    /** The \c StreamFactory class is responsible for creating and handling all the ers
      * streams used by the system. It implements the singleton pattern and the factory pattern. 
      * There should normally only be one instance of this class per process at any time, each instance 
      * handles a table of the different stream attached to each severity. 
      * When issues occurs, they can be dispatched using this instance.
      * Users should not use functions of this class apart from for registering new streams.
      * In order to report issues one should use global functions declared in the ers namespace.
      * In general, the following method should be used to dispatch issues
      * 
      * \author Serguei Kolos
      * \version 1.2
      * \brief Factory for Stream objects and repository of default streams. 
      * \see ers::Stream
      * \see ers::debug
      * \see ers::error
      * \see ers::fatal
      * \see ers::infomation
      * \see ers::warning
      */
    
    class StreamFactory
    {	
        typedef ers::Stream * (*StreamCreator)( const std::string & format );
      
      public:
	
        ~StreamFactory();
        
  	static StreamFactory & instance();		/**< \brief return the singleton */

	void debug( const Issue & issue, int level );	/**< \brief sends an Issue to the debug stream */
	void error( const Issue & issue );		/**< \brief sends an issue to the error stream */
	void fatal( const Issue & issue );		/**< \brief sends an issue to the fatal stream */
	void information( const Issue & issue );	/**< \brief sends an issue to the information stream */
	void warning( const Issue & issue );		/**< \brief sends an issue to the warning stream */

	void register_stream( const std::string & name, StreamCreator callback ); /**< \brief register a factory method */
	
        int debug_level() const 			/**< \brief returns current debug level */
        { return m_debug_level; }
        
        int verbosity_level() const 			/**< \brief returns current verbosity level */
        { return m_verbosity_level; }
        
        int & verbosity_level() 			/**< \brief can be used to set the current verbosity level */
        { return m_verbosity_level; }
        
      private:	
	StreamFactory( );

	Stream * get_stream( ers::severity severity );
	Stream * create_stream( const std::string & format, const std::string & default_key ) const;
	Stream * debug( int level );
        
	typedef std::map<std::string, StreamCreator> FunctionMap;
        
	ers::Stream *	m_streams[ers::Fatal + 1];	/**< \brief array of pointers to streams per severity */
	FunctionMap	m_factories;			/**< \brief collection of factories to build streams */	
    	int		m_debug_level;			/**< \brief current active level for the debug stream */	
    	int		m_verbosity_level;		/**< \brief current verbosity level for all streams */
    	ers::Stream *	m_null_stream;			/**< \brief this stream is used when no appropriate one is found */
    };
    
    std::ostream & operator<<( std::ostream &, const ers::StreamFactory & );
} // ers 

#endif

