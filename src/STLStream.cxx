/*
 *  STLStream.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 28.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */


#include <iostream>
#include <fstream>
#include <sstream>

#include "system/IOIssue.h"

#include "ers/STLStream.h"
#include "ers/ers.h"
#include "system/File.h"
#include "ers/XercesStream.h"
#include "ers/HumanStream.h"
#include "ers/TabStream.h"

const char* const ers::STLStream::CERR_STREAM_KEY = "cerr" ;        

ers::STLStream::STLStream() {
    open_stdin();
    open_stderr();
} // STLStream

ers::STLStream::STLStream(std::istream *in_stream) : ers::Stream() {
    ERS_PRE_CHECK_PTR(in_stream); 
    m_in_stream = in_stream ; 
    m_out_stream = 0 ; 
    m_delete_in_stream = false ; 
    m_delete_out_stream = false ; 
} // STLStream

ers::STLStream::STLStream(std::ostream *out_stream) : ers::Stream() {
    ERS_PRE_CHECK_PTR(out_stream); 
    m_in_stream =  0 ; 
    m_out_stream = out_stream ; 
    m_delete_in_stream = false ; 
    m_delete_out_stream = false ; 
} // STLStream

ers::STLStream::STLStream(const System::File &file, bool read_mode) {
    m_in_stream =  0 ; 
    m_out_stream = 0 ; 
    m_delete_in_stream = false ; 
    m_delete_out_stream = false ; 
    if (read_mode) { // read 
	open_read(file) ; 
    } else { // write 
	open_write(file) ; 
    } // write
} // STLStream

ers::STLStream::~STLStream() {
    if ((m_in_stream!=0) && m_delete_in_stream) delete m_in_stream ; 
    m_in_stream = 0 ; 
    if ((m_out_stream!=0) && m_delete_out_stream) delete m_out_stream ; 
    m_out_stream = 0 ;
} // STLStream

void ers::STLStream::open_stderr() {
    m_out_stream = &std::cerr ; 
    m_delete_out_stream = false ; 
} // open_stderr

void ers::STLStream::open_stdin() {
    m_in_stream = &std::cin ; 
    m_delete_in_stream = false ; 
} // open_stdin

void ers::STLStream::open_read(const System::File &file) {
    m_in_stream = file.input(); 
    m_delete_in_stream = true ;     
} // open_read

void ers::STLStream::open_write(const System::File &file) {
    m_out_stream = file.output(); 
    m_delete_out_stream = true ;     
} // open_write

/** Serialises the start of an issue. 
  * This method is called first 
  * \param issue_ptr the issue to serialize 
  * \note This implementation does nothing 
  */
void ers::STLStream::serialize_start(const ers::Issue *issue_ptr) { (void) issue_ptr ;  }   

/** Serialises the end of an issue. 
  * This method is called last.
  * \param issue_ptr the issue to serialize 
  * \note This implementation does nothing 
  */
void ers::STLStream::serialize_end(const ers::Issue *issue_ptr) { (void) issue_ptr ; } 

/** Serialises the separation between key-value pairs of an issue. 
  * This method is between each pair, it is called n-1 times where n is the number of key-pairs. 
  * \param issue_ptr the issue to serialize 
  * \note This implementation does nothing 
  */

void ers::STLStream::serialize_separator(const ers::Issue *issue_ptr) { (void) issue_ptr ; } 

/** Serialises a key-value pair. 
  * This method is called once per key-value pair
  * \param issue_ptr the issue to serialize 
  * \param key the key to store 
  * \param value the value to store
  * \note This implementation does nothing 
  */
void ers::STLStream::serialize_pair(const Issue* issue_ptr, const std::string &key, const std::string &value) { 
	(void) issue_ptr ; 
	(void) key ;
	(void) value ; 
} // serialize_pair

bool ers::STLStream::deserialize(string_map_type *values) { return false ;  }  



/** Sends a issue on the stream.
 * This method serialises an issue in the following way:
 * \li \c serialize_start is called
 * \li \c serialize is called with each key / value pair of the issue
 * \li \c each call to \c serialize is separated by a call to \c serialize_separator
 * \li \c serialize_end is called 
 * \param issue_ptr pointer to the issue to send
 * \pre issue_ptr is non null 
 * \pre internal stream is non null
 * \exception ers::InvalidReferenceIssue if issue_ptr is null
 * \exception ers::IOIssue if an output error occurs 
 */

void ers::STLStream::send(const Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    if (m_out_stream) {
	try {
	    const string_map_type *table = issue_ptr->get_value_table(); 
	    serialize_start(issue_ptr); 
	    bool first = true ; 
	    for(string_map_type::const_iterator pos = table->begin();pos!=table->end();++pos) {
		if (first) {
		    first = false ;
		} else {
		    serialize_separator(issue_ptr);
		} // first or not
		serialize_pair(issue_ptr,pos->first,pos->second);
	    } // for
	    serialize_end(issue_ptr); 
	} catch (std::ios_base::failure &ex) {
	    throw SYSTEM_IOERROR("Cannot write to stream",&ex); 
	} // catch generic I/O errors 
    } // if m_out_stream
} // send

/** This method implements the core of the deserialisation for Issues
  * It calls the  method \c deserialize to get the value table for the issue and builds it.
  * \see deserialize()
  * \return a newly created instance (heap allocated) or false if no data could be read. 
  */

ers::Issue *ers::STLStream::receive() {
    if (m_in_stream) {
	try {
	    string_map_type values ;
	    bool status=deserialize(&values);
	    if (status) {
		std::string class_name = values[Issue::CLASS_KEY] ; 
		Issue *i = ers::IssueFactory::instance()->build(class_name,&values); 
		return i ;
	    } // if
	} catch (std::ios_base::failure &ex) {
	    throw SYSTEM_IOERROR("failure while parsing",&ex);
	} // catch
    } // if
    return 0 ; 
} // receive

