/*
 *  Stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#ifndef __ERS_ISSUE_STREAM__
#define __ERS_ISSUE_STREAM__

#include <string>

#include "ers/Core.h"
#include "ers/Context.h"
#include "system/File.h"

namespace ers {
    
    class Issue ; 

/** Root issue stream.
  * This class is meant to be subclassed in order to implement concrete issue streams. 
  * \author Matthias Wiesmann
  * \version 1.0
  */
    
class Stream {
    friend class Issue ; 
protected:
    static Stream *s_streams[ers_severity_max] ;  
    static Stream *create_stream(const char* key); 
    static Stream *create_stream(ers_severity s) ; 
    static Stream *get_stream(ers_severity s); 
public:
    static const char* FILE_KEY ; 
    static const char* NULL_STREAM_KEY ; 
    static const char* CERR_STREAM_KEY ;
    static const char* XML_SUFFIX ; 
    static const char* TAB_SUFFIX ; 
    
    static Stream *factory(const System::File &file) ; 
    static Stream *factory(std::ostream *s, const std::string &type) ; 

    static const char *key_for_severity(ers_severity s) ; 
    
    static Stream *fatal();
    static void fatal(Issue *i) ;
    
    static Stream* error() ;                                                  /**< \brief the error stream */
    static void error(Issue *i) ;                                             /**< \brief sends an error to the error stream */
    static Stream* warning();                                                 /**< \brief the warning stream */
    static void warning(Issue *i);                                            /**< \brief sends a warning to the warning stream */
    static void warning(const Context &c, const std::string &message);        /**< \brief sends a warning message */
    static Stream* debug(ers_severity s) ;                                    /**< \brief the debug stream for a debug level */
    static void debug(Issue *i, ers_severity) ;                               /**< \brief sends a debug to the debug stream */
    static void debug(const Context &c, const std::string &message, ers_severity s);  /**< \brief sends a debug message */
    static void dispatch(Issue *i, bool throw_error = false) ;                /**< \brief Sends an issue to the appropriate stream according to its severity */
    
    Stream();
    virtual ~Stream(); 
    virtual void send(const Issue *i) ;          /**< \brief Sends an issue into the stream */
    virtual Issue *receive() ;                   /**< \brief Receives an issue from the stream */
      
} ; 

} // ers 

#define ERS_DEBUG_0(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_0) ; }
#define ERS_DEBUG_1(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_1) ; }
#define ERS_DEBUG_2(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_2) ; }
#define ERS_DEBUG_3(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers::ers_debug_3) ; }
#define ERS_WARN(...)    { char ers_warn_buf[256] ; snprintf(ers_warn_buf,sizeof(ers_warn_buf),__VA_ARGS__)    ; ers::Stream::warning(ERS_HERE,ers_warn_buf) ; }




#endif

