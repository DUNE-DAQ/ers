/*
 *  SyslogStream.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 31.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/SyslogStream.h"
#include "ers/ers.h"

#include <syslog.h>
#include <stdarg.h>


ers::SyslogStream::SyslogStream() {}
ers::SyslogStream::~SyslogStream() {}


/** Converts the severity of an Issue into a gravity level suitable for syslog.
 * All log levels are converted to \c LOG_DEBUG \c ers_information and \c ers_notification to \c LOG_INFO
 * \c ers_warning to \c LOG_WARNING and both \c ers_error and \c ers_fatal to \c LOG_ERR.
 * All other values return \c LOG_INFO
 * \param s the ers severity to convert 
 * \return a gravity level for syslog. 
 * \note for more info see the \c SYSLOG(3) manual. 
 */

int ers::SyslogStream::syslog_level(ers_severity s) {
    switch(s) {
	case ers_debug_0:
	case ers_debug_1:
	case ers_debug_2:
	case ers_debug_3: 
	    return LOG_DEBUG ; 
	case ers_information:
	case ers_notification:
	    return LOG_INFO ; 
	case ers_warning:
	    return LOG_WARNING ; 
	case ers_error:
	case ers_fatal:
	    return LOG_ERR ; 
	default:
	    return LOG_INFO ;
    }  // switch    
} // syslog_level

/** Sends an issue to the stream 
  * \param issue_ptr the issue to send
  */

void ers::SyslogStream::send(const ers::Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr);
    std::string message = issue_ptr->human_description();
    const char *msg = message.c_str(); 
    const int level = syslog_level(issue_ptr->severity()); 
    ::syslog(level,"%s",msg); 
} // send



