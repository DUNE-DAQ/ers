/*
 *  MessageStream.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 25.02.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/MessageStream.h"
#include "ers/StreamFactory.h"
#include "ers/Issue.h"

const char* const ers::MessageStream::SUFFIX = "msg" ; 

namespace {
    ers::Stream *create_stream(const std::string &protocol, const std::string &uri) { 
	if (protocol==System::File::FILE_PROTOCOL) {
	    System::File file(uri); 
	    std::string extension = file.extension(uri) ;
	    if (extension==ers::MessageStream::SUFFIX) return new ers::MessageStream(file); 
	} // tab file
	if ((protocol==ers::STLStream::CERR_STREAM_KEY) && (uri==ers::MessageStream::SUFFIX)) {
	    return new ers::MessageStream(&std::cerr); 
	} // tab stream
	return 0 ;
    } // 
    bool registered = ers::StreamFactory::instance()->register_factory(ers::MessageStream::SUFFIX,create_stream) ;
} // anonymous namespace


ers::MessageStream::MessageStream() : STLStream() {} 

ers::MessageStream::MessageStream(std::ostream *out_stream) : STLStream(out_stream) {}

ers::MessageStream::MessageStream(const System::File &file) : STLStream(file,false) {} 

ers::MessageStream::~MessageStream() {} 

void ers::MessageStream::send(const Issue *issue) {
    const std::string & msg = issue->message(); 
    (*m_out_stream) << msg << std::endl ; 
} // send
