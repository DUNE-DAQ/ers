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
#include "ers/InvalidReferenceIssue.h"

/** Builds an ERS stream that writes into a file 
  * \param filename the name of the file
  * \pre filename is non null
  */

ers::STLOutStream::STLOutStream(const char* filename) {
    ERS_PRE_CHECK_PTR(filename); 
    try {
	this->m_stream = new std::ofstream(filename) ; 
	m_stream->exceptions(std::ios::failbit | std::ios::badbit); 
	m_delete_stream = true ; 
    } catch (std::ios_base::failure &ex) {
	throw ERS_OPEN_WRITE_FAIL(filename,&ex); 
    } // catch
} // Stream_Stream

/** Builds an ERS stream that writes into a STL stream 
 * \param s the STL stream to write into
 * \pre s is non null
 */

ers::STLOutStream::STLOutStream(std::ostream *s) {
    ERS_PRE_CHECK_PTR(s); 
    this->m_stream = s ; 
    this->m_delete_stream = false ;
} // STLOutStream

/** Builds an ERS stream that writes into the standard error stream
 */

ers::STLOutStream::STLOutStream() {
    m_stream = &std::cerr ;
    m_delete_stream = false ; 
} // Stream_Stream

/** Destroy the ERS stream 
  * \pre internal stream is non null 
  * \post internal stream is null 
  */

ers::STLOutStream::~STLOutStream() {
    ERS_PRE_CHECK_PTR(m_stream); 
    if (m_delete_stream) {
	delete(m_stream); 
    } // if 
    m_stream = 0 ; 
} // ~Stream_Stream

/** Sends a issue on the stream.
  * This method serialises an issue in the following way:
  * \li \c serialize_start is called
  * \li \c serialize is called with each key / value pair of the issue
  * \li \c each call to \c serialize is separated by a call to \c serialize_separator
  * \li \c serialize_end is called 
  * \param i pointer to the issue to send
  * \pre i is non null 
  * \pre internal stream is non null
  * \exception ers::Precondition if precondition is not fullfilled
  * \exception ers::IOIssue if an output error occurs 
  */

void ers::STLOutStream::send(const Issue *i) {
    ERS_PRE_CHECK_PTR(i); 
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

/** Method to receive issues
  * \attention This method is not implemented and alway return 0 
  * \return 0 
  */

ers::Issue *ers::STLOutStream::receive() {
    return 0 ;
} // receive

/** Default implementation for \c serialize_separator does nothing 
  * \param i the issue to serialize
  * \pre issue is non null 
  */

void ers::STLOutStream::serialize_separator(const ers::Issue *i) {
     ERS_PRE_CHECK_PTR(i);
} // serialize_separator

/** Default implementation for \c serialize_start does nothing 
  * \param i the issue to serialize
  * \pre issue is non null 
  */

void ers::STLOutStream::serialize_start(const ers::Issue *i) {
     ERS_PRE_CHECK_PTR(i);
} // serialize_start

/** Default implementation for \c serialize_start does nothing 
  * \param i the issue to serialize
  * \pre issue is non null 
  */

void ers::STLOutStream::serialize_end(const Issue *i) {
    ERS_PRE_CHECK_PTR(i);
} // serialize_end


