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
#include "ers/InvalidReferenceIssue.h"

#include "system/File.h"
#include "system/OpenFail.h"

using namespace System ; 

ers::STLInStream::STLInStream(std::istream *s) {
    ERS_PRE_CHECK_PTR(s); 
    this->m_stream = s ; 
    this->m_delete_stream = false ;
} // STLInStream

ers::STLInStream::STLInStream(const char* filename) {
    open(filename);
} // STLInStream

ers::STLInStream::STLInStream(const std::string &filename) {
    open(filename.c_str());
} // STLInStream

ers::STLInStream::STLInStream(const File& file) {
    open(file.full_name().c_str());
} // STLInStream


ers::STLInStream::STLInStream() {
} // STLInStream

ers::STLInStream::~STLInStream() {
    ERS_PRE_CHECK_PTR(m_stream); 
    if (m_delete_stream) {
	delete(m_stream); 
    } // if 
    m_stream = 0 ; 
} // ~STLInStream

void ers::STLInStream::open(const char* filename) {
    ERS_PRE_CHECK_PTR(filename); 
    try {
	this->m_stream = new std::ifstream(filename) ; 
	m_stream->exceptions(std::ios::failbit | std::ios::badbit); 
	m_delete_stream = true ; 
    } catch (std::ios_base::failure &ex) {
	throw ERS_OPEN_READ_FAIL(filename,&ex); 
    } // catch
} // open


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

