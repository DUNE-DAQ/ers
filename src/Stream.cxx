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
#include "ers/InvalidReferenceIssue.h"
#include "ers/Precondition.h"
#include "ers/LogIssue.h"

using namespace ers ; 

Stream *Stream::s_streams[] = { 0 } ; 

Stream* Stream::error() {
    if (! s_streams[ers_error]) {
	s_streams[ers_error] = new TabOutStream(); 
    } // if
    return s_streams[ers_error] ; 
} // error

Stream* Stream::debug(ers_severity s) {
    ERS_PRECONDITION(s<ers_information,"severity is not debug : %s (%d) %d",get_severity_text(s),s,ers_information);
    if (! s_streams[s]) {
	s_streams[s] = new TabOutStream(); 
    } // if
    return s_streams[s] ; 
} // debug


void Stream::debug(const Issue *i, ers_severity s) {
    ERS_PRE_CHECK_PTR(i); 
    Stream *stream = debug(s); 
    stream->send(i); 
} //  

void Stream::debug(const Context &c, const std::string &message, ers_severity s) {
    LogIssue i(c,s,message); 
    debug(&i,s); 
} // debug

void Stream::error(const Issue *i) {
    ERS_PRE_CHECK_PTR(i); 
    error()->send(i); 
} // error




