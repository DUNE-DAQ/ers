/*
 *  Stream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <assert.h>

#include "ers/Issue.h"
#include "ers/StreamFactory.h"

const char* ers::Stream::NULL_STREAM_KEY = "null" ; 

namespace {
    ers::Stream *create_stream(const std::string &protocol, const std::string &uri) { 
	if (protocol==ers::Stream::NULL_STREAM_KEY) return new ers::Stream() ; 
	return 0 ;
    } 
    bool registered = ers::StreamFactory::instance()->register_factory(create_stream) ;
} 





// --------------
// Member methods 
// --------------

ers::Stream::Stream() {}
ers::Stream::~Stream() {}

/** Sends the issue to the stream.
  * \param i the issue to send 
  * \note This implementation silently discards the Issue
  */

void ers::Stream::send(const Issue *i) {
    (void) i ; 
} // send 

/** Reads an isssue on the stream
  * \return the issue that has been read
  * \note This implementation always returns a null pointer
  */

ers::Issue *ers::Stream::receive() {
    return 0 ; 
} // receive




