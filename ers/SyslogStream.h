/*
 *  SyslogStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 31.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_SYSLOGSTREAM
#define ERS_SYSLOGSTREAM

#include "ers/Stream.h"

namespace ers {
    
    /** This streams sends the issues into syslog. 
      * The data is sent in the human readable format returned by method \c Issue::human_description()
      * \see ers::Issue::human_description()
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief Stream to send data into syslog
      */
    
    class SyslogStream : public Stream {
public:
	static const char* const SYSLOG_KEY ;          /**< Key for syslog stream */
	static int syslog_level(ers_severity s) ; 
	static int syslog_level(const Issue *issue_ptr) ; 
	SyslogStream();
	~SyslogStream() ; 
	virtual void send(const Issue *issue_ptr) ;   
    } ; // SyslogStream
} // ers

#endif
