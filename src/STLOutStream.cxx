/*
 *  STLOutStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/STLOutStream.h"
#include "ers/Precondition.h"
#include "ers/OpenFail.h"

/** Builds a stream that writes into a file 
  * @param filename the name of the file
  */

ers::STLOutStream::STLOutStream(const char* filename) {
    ERS_PRECONDITION(filename!=0,"Null pointer for filename"); 
    try {
	this->m_stream = new std::ofstream(filename) ; 
	m_stream->exceptions(std::ios::failbit | std::ios::badbit); 
	m_delete_stream = true ; 
    } catch (std::ios_base::failure &ex) {
	throw ERS_OPEN_WRITE_FAIL(filename,&ex); 
    } // catch
} // Stream_Stream

ers::STLOutStream::STLOutStream(std::ostream *s) {
    ERS_PRECONDITION(s!=0,"Null pointer for stream"); 
    this->m_stream = s ; 
    this->m_delete_stream = false ;
} // STLOutStream

ers::STLOutStream::STLOutStream() {
    m_stream = &std::cout ;
    m_delete_stream = false ; 
} // Stream_Stream

ers::STLOutStream::~STLOutStream() {
    ERS_PRECONDITION(m_stream,"Destructor called with null stream");
    if (m_delete_stream) {
	delete(m_stream); 
    } // if 
    m_stream = 0 ; 
} // ~Stream_Stream

void ers::STLOutStream::send(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue pointer"); 
    try {
	const string_map_type *table = i->get_value_table(); 
	serialize_start(i); 
	bool first = true ; 
	for(string_map_type::const_iterator pos = table->begin();pos!=table->end();++pos) {
	    if (first) {
		first = false ;
	    } else {
		serialize_separator(i);
	    } // first or not
	    serialize(pos->first,pos->second);
	} // for
	serialize_end(i); 
    } catch (std::ios_base::failure &ex) {
	throw ERS_IOERROR("Cannot write to stream",&ex); 
    } 
} // send

ers::Issue *ers::STLOutStream::receive() {
    return 0 ;
} // receive

void ers::STLOutStream::serialize_separator(const ers::Issue *i) {
     ERS_PRECONDITION(i!=0,"Null Issue pointer");
} // serialize_separator

void ers::STLOutStream::serialize_start(const ers::Issue *i) {
     ERS_PRECONDITION(i!=0,"Null Issue pointer");
} // serialize_start

void ers::STLOutStream::serialize_end(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue pointer");
} // serialize_end


