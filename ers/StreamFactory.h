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

#include "ers/Core.h"
#include "ers/Context.h"

#include <map>

namespace ers {
    
    class Stream ; 
    class Issue ; 
    
    /** The \c StreamFactory class is responsible for creating and handling all the ers stream used by the system. 
      * It implements the singleton pattern and the factory pattern. 
      * There should normally only be one instance of this class per process at any time, each instance 
      * handles a table of the different stream attached to each severity. 
      * When issues occurs, they can be dispatched using this instance. 
      * In general, the following method should be used to dispatch issues<ul>
      * <li> \c dispatch if the severity_t of the issue is set correctly. </li>
      * <li> \c fatal, \c error, \c warning, \c debug for setting the severity_t and dispatch.</li>
      * </ul>
      * The default stream are set up in the following way: 
      * <ol>
      * <li>The system checks if an environnement variable with the same name as severity_t is defined. 
      *     If this is the case, the value of this variable is parsed as a key for building the stream.
      *     Those key are basically URLs, either they start with the \c file protocol, or a keyword that
      *     identifies the stream \c cerr: is the standard ou stream, null: is the discarding a stream, 
      *     fifo is a queue based stream. The format used to serialise issues in the stream is determined by 
      *     the file extension in case of a file, or by the code after the colon. 
      *     So for instance file:/tmp/out.xml means that the stream writes
      *     issues into file /tmp/out.xml in the XML format, cerr:tab mean that issues
      *     are streamed as tab data into stderr. </li>
      * <li>If the stream cannot be determined out of the environement variable, a default stream is built. 
      *     The actual type of this default stream is determined by the content of the key in \c DEFAULT_STREAMS.
      *     The content is a key with the format described above.y</li>
      * <li>The user can provide another stream for any severity_t simply by calling method \c set, either 
      *     by specifying the stream directly, or by providing a key.</li>
      * </ol>
      * The string format used by key supports the following protocols by default:<ul>
      * <li>null: issues are discarded</li>
      * <li>default: issues are displayed on standard error stream</li>
      * <li>fifo: writes into a FIFO stream, to be actually used the issues need to be read by another entity</li>
      * </ul>
      * More streams are offered by different packages, to make those stream availables link in the relevant packages:<br>
      * <b>System</b> package: <ul>
      * <li>file: writes into a file</li>
      * <li>cerr: writes into the standard error stream</li>
      * <li>syslog: issues are sent to syslog</li>
      * </ul> 
      * The string format supports the following serializing encodings for cerr and file streams<ul>
      * <li>XML - issues are serialized as XML data</li>
      * <li>TAB - issues are serialized as tabulated data</li>
      * <li>TXT - issues are serialized as single line of text</li>
      * </ul>
      * <b>ersipc</b> package: <ul>
      * <li>mrs: writes issues into the MRS system (q.v.) the rest of the uri in this case designates the IPC partition to use
      *     for instance mrs://data
      * In order to add new stream implementations, one needs to register a factory function of type \c create_stream_callback.
      * This method takes two parameters, a protocol parameter, and an uri parameter, that basically contains everything except
      * the protocol part of the key. If the implementation can offer a stream that corresponds to the key it should return a 
      * heap allocated stream, if not it should return null. This method should be registered using the \c register_factory method
      * on the default instance. 
      * \author Matthias Wiesmann
      * \version 1.1
      * \brief Factory for Stream objects and repository of default streams. 
      */
    
    
    class StreamFactory {
	
public:
	typedef Stream* (*create_stream_callback)(const std::string &, const std::string &);
	typedef std::map<std::string, create_stream_callback> stream_factory_collection ; 
protected:
	static StreamFactory *s_instance ;                                        /**< \brief singleton instance */
	static const char* DEFAULT_STREAMS[] ;                                    /**< \brief keys for default streams */
	static const char *key_for_severity(severity_t s) ;                     /**< \brief finds key for severity_t */
	static Stream *get_default_stream(severity_t s) ;                       /**< \brief builds default stream for severity_t */
	
	StreamFactory();
	StreamFactory(const StreamFactory &other); 
	
	Stream *m_streams[severity_max] ;                                     /**< \brief array of pointers to streams per severity_t */
	stream_factory_collection m_factories ;                                   /**< \brief collection of factories to build streams */
	
	Stream *create_stream(severity_t s) ;                                   /**< \brief create a stream for severity_t */
public:
	static StreamFactory *instance();                                         /**< \brief return the singleton */
	static void print_registered(); 
	static void fatal(Issue *i) ;                                             /**< \brief sends an issue to the fatal stream */
	static void error(Issue *i) ;                                             /**< \brief sends an issue to the error stream */
	static void warning(Issue *i);                                            /**< \brief sends an issue to the warning stream */
	static void warning(const Context &c, const std::string &message);        /**< \brief sends a warning message */
	static void debug(Issue *i, severity_t) ;                               /**< \brief sends an Issue to the debug stream */
	static void debug(const Context &c, const std::string &message, severity_t s);  /**< \brief sends a debug message */
	static void dispatch(Issue *i, bool throw_error = false) ;                /**< \brief Sends an issue to the appropriate stream according to its severity_t */
	static void dispatch(Issue &i, bool throw_error = false) ; 	
	static void set_stream(severity_t, const std::string &key) ;
	Stream *create_stream(const std::string &key) const ;                         /**< \brief create a stream from a key */
	Stream *get_stream(severity_t s) ;                                      /**< \brief get stream for severity_t */
	void set(severity_t severity, Stream *s) ;                              /**< \brief Sets the stream for a given severity_t */
	void set(severity_t severity, const char* key) ;                        /**< \brief Setup a stream for a given severity_t based on a key */
	Stream *fatal() ;                                                         /**< \brief Fatal stream */
	Stream *error()  ;                                                        /**< \brief Error stream */
	Stream *warning()  ;                                                      /**< \brief Warning stream */
	Stream* debug(severity_t s)  ;                                          /**< \brief Debug stream for level*/
	bool register_factory(const std::string &name, create_stream_callback callback); /**< \brief register a factory method */
	void write_to(std::ostream& stream) const ;                               /**< \brief write content of factory to stream */
    } ; 
    std::ostream& operator<<(std::ostream&, const ers::StreamFactory& factory);        /**< \brief streaming operator */
} // ers 

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 3
#endif

    
    /** Sends a debug message with level 0, the first parameter is a \c printf like pattern, the next are parameters for it */
    
#if ! defined(ERS_FAST_DEBUG)
#define ERS_DEBUG_0(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::debug_0) ; }
#else 
#define ERS_DEBUG_0(...) { fprintf(stderr,"debug-0 ") ; fprintf(stderr,__VA_ARGS__) ; fprintf(stderr,"\n") ; }
#endif
    
    
    /** Sends a debug message with level 1, the first parameter is a \c printf like pattern, the next are parameters for it */
#if (DEBUG_LEVEL>0) 
#if (! defined(ERS_FAST_DEBUG))
#define ERS_DEBUG_1(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::debug_1) ; }
#else 
#define ERS_DEBUG_1(...) { fprintf(stderr,"debug-1 ") ; fprintf(stderr,__VA_ARGS__) ; fprintf(stderr,"\n") ; }
#endif
#else 
#define ERS_DEBUG_1(...) 
#endif
    
    /** Sends a debug message with level 2, the first parameter is a \c printf like pattern, the next are parameters for it */
#if DEBUG_LEVEL>1
#define ERS_DEBUG_2(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::debug_2) ; }
#else 
#define ERS_DEBUG_2(...)
#endif
    
    /** Sends a debug message with level 3, the first parameter is a \c printf like pattern, the next are parameters for it */
#if DEBUG_LEVEL>2
#define ERS_DEBUG_3(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::StreamFactory::debug(ERS_HERE,ers_debug_buf,ers::debug_3) ; }
#else 
#define ERS_DEBUG_3(...) 
#endif
    
    
    /** Sends a warning, the first parameter is a \c printf like pattern, the next are parameters for it */
#define ERS_WARN(...)    { char ers_warn_buf[256] ; snprintf(ers_warn_buf,sizeof(ers_warn_buf),__VA_ARGS__)    ; ers::StreamFactory::warning(ERS_HERE,ers_warn_buf) ; }
    
    
    

#endif

