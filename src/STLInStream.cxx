/*
 *  STLInStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 09.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/STLInStream.h"
#include "ers/Issue.h"
#include "ers/IssueFactory.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"
#include "ers/OpenFail.h"


ers::STLInStream::STLInStream(std::istream *s) {
    ERS_PRECONDITION(s!=0,"Null pointer for stream"); 
    this->m_stream = s ; 
    this->m_delete_stream = false ;
} // STLInStream

ers::STLInStream::STLInStream(const char* filename) {
    ERS_PRECONDITION(filename!=0,"Null pointer for filename"); 
    try {
	this->m_stream = new std::ifstream(filename) ; 
	m_stream->exceptions(std::ios::failbit | std::ios::badbit); 
	m_delete_stream = true ; 
    } catch (std::ios_base::failure &ex) {
	throw ERS_OPEN_READ_FAIL(filename,&ex); 
    } // catch
} // STLInStream

ers::STLInStream::STLInStream() {
} // STLInStream

ers::STLInStream::~STLInStream() {
    ERS_PRECONDITION(m_stream,"Destructor called with null stream");
    if (m_delete_stream) {
	delete(m_stream); 
    } // if 
    m_stream = 0 ; 
} // ~STLInStream

/** This method implements the core of the deserialisation for Issues
  * It calls the abstract method \c read_properties to get the value table for the issue and builds it.
  * @see read_properties()
  * @return a newly created instance
  */

ers::Issue *ers::STLInStream::receive() {
    try {
	string_map_type values ;
	bool status=read_properties(&values);
	if (status) {
	    std::string class_name = values[Issue::CLASS_KEY] ; 
	    Issue *i = ers::IssueFactory::instance()->build(class_name,&values); 
	    return i ; 
	} else {
	    return 0 ; 
	} // if read worked
    } catch (std::ios_base::failure &ex) {
	throw ERS_IOERROR("failure while parsing",&ex);
    } 
} // receive

/** This method is not implemented, this is an input stream 
  */

void ers::STLInStream::send(const Issue *i) {
    NOT_IMPLEMENTED() ; 
} // send

