/*
 *  Stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include <string>

#include "ers/Core.h"
#include "ers/Context.h"

#ifndef __ERS_ISSUE_STREAM__
#define __ERS_ISSUE_STREAM__

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
    
public:
    static Stream* error() ;
    static Stream* debug(ers_severity s) ; 
    static void debug(const Issue *i, ers_severity) ; 
    static void debug(const Context &c, const std::string &message, ers_severity s); 
	static void error(const Issue *i) ; 
    
    virtual void send(const Issue *i) = 0;          /**< \brief Sends an issue into the stream */
    virtual Issue *receive() = 0; 
    virtual ~Stream() {} 
      
} ; 

} // ers 

#define ERS_DEBUG_0(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers_debug_0) ; }
#define ERS_DEBUG_1(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers_debug_0) ; }
#define ERS_DEBUG_2(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers_debug_0) ; }
#define ERS_DEBUG_3(...) { char ers_debug_buf[256] ; snprintf(ers_debug_buf,sizeof(ers_debug_buf),__VA_ARGS__) ; ers::Stream::debug(ERS_HERE,ers_debug_buf,ers_debug_0) ; }




#endif

