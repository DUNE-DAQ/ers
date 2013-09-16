/*
 *  StreamManager.h
 *  ers
 *
 *  Created by Serguei Kolos on 21.11.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_STREAM_MANAGER_H
#define ERS_STREAM_MANAGER_H

#include <initializer_list>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <ers/Severity.h>
#include <ers/Context.h>
#include <ers/IssueReceiver.h>
#include <ers/StreamFactory.h>
#include <ers/internal/PluginManager.h>

#include <list>

/** \file StreamManager.h This file defines the StreamManager class, 
  * which is responsible for manipulation of ERS streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

namespace ers
{    
    class InputStream; 
    class LocalStream;
    class OutputStream; 
    class ErrorHandler; 
    class Issue;
    class StreamInitializer;
    template <class > class SingletonCreator;
    
    /** The \c StreamManager class is responsible for creating and handling all the ers
      * streams used by an application. It implements the singleton pattern and handles a 
      * table of the different stream attached to each severity. 
      * When issues occurs, they can be dispatched using this instance.
      * Users should not use functions of this class directly.
      * In order to report issues one should use global functions declared in the ers namespace.
      * 
      * \author Serguei Kolos
      * \version 1.2
      * \brief Manager of ERS streams. 
      * \see ers::debug
      * \see ers::error
      * \see ers::fatal
      * \see ers::infomation
      * \see ers::warning
      */
    
    class StreamManager
    {      
      friend class StreamInitializer;
      friend class ers::LocalStream;
      friend class ers::ErrorHandler;
      template <class > friend class SingletonCreator;
      
      public:
	
        ~StreamManager();
        
  	static StreamManager & instance();		/**< \brief return the singleton */
        
	void debug( const Issue & issue, int level );	/**< \brief sends an Issue to the debug stream */
	
        void error( const Issue & issue );		/**< \brief sends an issue to the error stream */
	
        void fatal( const Issue & issue );		/**< \brief sends an issue to the fatal stream */
	
        void information( const Issue & issue );	/**< \brief sends an issue to the information stream */
	
        void log( const Issue & issue );		/**< \brief sends an issue to the log stream */
        
        void warning( const Issue & issue );		/**< \brief sends an issue to the warning stream */

        void add_receiver(	const std::string & stream,
        			const std::string & filter,
                                ers::IssueReceiver * receiver ) throw ( ers::InvalidFormat );
	
        void add_receiver(	const std::string & stream,
        			const std::initializer_list<std::string> & params,
                                ers::IssueReceiver * receiver ) throw ( ers::InvalidFormat );
	
        void remove_receiver( ers::IssueReceiver * receiver );
	
        void add_output_stream( ers::severity severity, ers::OutputStream * new_stream );	
      
	void report_issue( ers::severity type, const Issue & issue );

      private:	
	StreamManager( );

	OutputStream * setup_stream( ers::severity severity );	
	OutputStream * setup_stream( const std::vector<std::string> & streams );
        
	PluginManager					m_plugin_manager;
	boost::mutex					m_mutex;
	std::list<boost::shared_ptr<InputStream> >	m_in_streams;               
	boost::shared_ptr<OutputStream>			m_init_streams[ers::Fatal + 1];	/**< \brief array of pointers to streams per severity */
	boost::shared_ptr<OutputStream>			m_out_streams[ers::Fatal + 1];	/**< \brief array of pointers to streams per severity */
    };
    
    std::ostream & operator<<( std::ostream &, const ers::StreamManager & );
}

#endif

