/*
 *  StreamFactory.h
 *  Test
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/Core.h"
#include "ers/Context.h"
#include "system/File.h"

namespace ers {
    
    class Stream ; 
    class Issue ; 
    
    /** The \c StreamFactory class is responsible for creating and handling all the ers stream used by the system. 
      * It implements the singleton pattern and the factory pattern. 
      * There should normally only be one instance of this class per process at any time, each instance 
      * handles a table of the different stream attached to each severity. 
      * When issues occurs, they can be dispatched using this instance. 
      * In general, the following method should be used to dispatch issues 
      * \li \c dispatch if the severity of the issue is set correctly. 
      * \li \c fatal, \c error, \c warning, \c debug for setting the severity and dispatch.
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
    
    class StreamFactory {
protected:
	static StreamFactory *s_instance ; 
	static const char* DEFAULT_STREAMS[] ; 
	static const char *key_for_severity(ers_severity s) ; 
	static Stream *get_default_stream(ers_severity s) ; 
	StreamFactory();
	Stream *m_streams[ers_severity_max] ;  
	Stream *create_stream(const char* key) ; 
	Stream *create_stream(ers_severity s) ; 
	Stream *get_stream(ers_severity s) const ; 
public:
	static const char* FILE_KEY ; 
	static const char* NULL_STREAM_KEY ; 
	static const char* CERR_STREAM_KEY ;
	static const char* FIFO_STREAM_KEY ; 
	static const char* XML_SUFFIX ; 
	static const char* TAB_SUFFIX ; 
	static StreamFactory *instance(); 
	static void fatal(Issue *i) ; 
	static void error(Issue *i) ;                                             /**< \brief sends an error to the error stream */
	static void warning(Issue *i);                                            /**< \brief sends a warning to the warning stream */
	static void warning(const Context &c, const std::string &message);        /**< \brief sends a warning message */
	static void debug(Issue *i, ers_severity) ;                               /**< \brief sends a debug to the debug stream */
	static void debug(const Context &c, const std::string &message, ers_severity s);  /**< \brief sends a debug message */
	static void dispatch(Issue *i, bool throw_error = false) ;                /**< \brief Sends an issue to the appropriate stream according to its severity */	
	static Stream *factory(std::ostream *s, const std::string &type) ; 
	static Stream *factory(const System::File &file)  ; 
	Stream *fatal() const;
	Stream *error() const ; 
	Stream *warning() const ;                                             
	Stream* debug(ers_severity s) const ;                 
    } ; 
    
#define ERS_DEBUG_0(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_0) ; }
#define ERS_DEBUG_1(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_1) ; }
#define ERS_DEBUG_2(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_2) ; }
#define ERS_DEBUG_3(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_3) ; }
#define ERS_WARN(...)    { char ers_warn_buf[256] ; snprintf(ers_warn_buf,sizeof(ers_warn_buf),__VA_ARGS__)    ; ers::StreamFactory::warning(ERS_HERE,ers_warn_buf) ; }
    
    
    
} // ers 
