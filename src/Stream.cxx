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
#include "ers/TabOutStream.h"

#ifdef USE_TINY_XML
#include "ers/TinyXMLStream.h"
#else 
#include "ers/XercesStream.h"
#endif
#include "ers/HumanStream.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/Precondition.h"
#include "ers/LogIssue.h"

using namespace ers ; 

const char *Stream::FILE_KEY = "file" ; 
const char *Stream::NULL_STREAM_KEY = "null" ;
const char *Stream::CERR_STREAM_KEY = "cerr" ; 
const char* Stream::XML_SUFFIX = "xml" ; 
const char* Stream::TAB_SUFFIX = "tab" ; 

Stream* Stream::s_streams[] = { 0 } ; 

/** This function builds a streams that writes into a file.
  * The streaming format is decided by the suffix of the file. 
  * \param file File to write into 
  */

Stream *Stream::factory(const System::File &file) {
    if (file.extension()==XML_SUFFIX) {
    	return new ers::XercesStream(file); 
     }
    if (file.extension()==TAB_SUFFIX) {
        return new TabOutStream(file); 
    }
    return new HumanStream(file); 
} // path_factory

/** This function builds a stream that writes into a STL stream
  * The streaming format is decide by parameter \c type
  * \param s the stream to stream into
  * \param type the type of streaming (xml or tab). 
  */

Stream* Stream::factory(std::ostream *s, const std::string &type)  {
    if (type==TAB_SUFFIX) return new TabOutStream(s); 
    return new TabOutStream(s); 
} // stream_factory

const char* Stream::key_for_severity(ers_severity s) {
    char key_buffer[64] ; 
    snprintf(key_buffer,sizeof(key_buffer),"ERS_%s",get_severity_text(s)) ;
    char *c = &(key_buffer[0]);
    while(*c) {
	*c = toupper(*c) ;
	c++ ;
    } // while
    const char *env = ::getenv(key_buffer) ; 
    if (env!=0) return env ;
    return CERR_STREAM_KEY ; 
} // key_for_severity

Stream *Stream::create_stream(const char* c_key) {
    if (c_key==0) return new Stream(); 
    std::string key = std::string(c_key); 
    std::string protocol = System::File::protocol(key);
    if (protocol==FILE_KEY) return factory(System::File::from_url(key));
    if (protocol==NULL_STREAM_KEY) return new Stream(); 
    if (protocol==CERR_STREAM_KEY) return factory(&std::cerr,System::File::uri(key)) ; 
    return new HumanStream(&std::cerr); 
} // create_stream

Stream *Stream::create_stream(ers_severity c) {
    const char* key = key_for_severity(c); 
    return create_stream(key); 
} // get_stream

Stream *Stream::get_stream(ers_severity s) {
    if (! s_streams[s]) {
	s_streams[s]=create_stream(s) ; 
    } // if
    ERS_ASSERT(s_streams[s]!=0,"stream not built for severity : %s", get_severity_text(s));
    return s_streams[s] ; 
} // get_stream

Stream* Stream::error() {
    return get_stream(ers_error) ; 
} // error

Stream* Stream::debug(ers_severity s) {
    ERS_PRECONDITION(s<ers_information,"severity is not debug : %s (%d) %d",get_severity_text(s),s,ers_information);
    return get_stream(s) ; 
} // debug

void Stream::debug(Issue *i, ers_severity s) {
    ERS_PRE_CHECK_PTR(i); 
    ERS_PRECONDITION(s<ers_information,"severity is not debug : %s (%d) %d",get_severity_text(s),s,ers_information);
    i->severity(s) ; 
    Stream *stream = debug(s); 
    stream->send(i); 
} //  debug

void Stream::debug(const Context &c, const std::string &message, ers_severity s) {
    LogIssue i(c,s,message); 
    debug(&i,s); 
} // debug

void Stream::dispatch(Issue *issue_ptr, bool throw_error) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    if (throw_error && issue_ptr->is_error()) { throw *issue_ptr  ; } 
    const ers_severity s = issue_ptr->severity() ;
    Stream *stream_ptr =  s_streams[s] ; 
    stream_ptr->send(issue_ptr); 
} // dispatch

void Stream::error(Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    error()->send(issue_ptr); 
} // error

// Member methods 
// --------------

Stream::Stream() {}
Stream::~Stream() {}

void Stream::send(const Issue *i) {
    (void) i ; 
} // send 

Issue *Stream::receive() {
    return 0 ; 
} // receive




