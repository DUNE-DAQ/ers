/*
 *  TabOutStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <assert.h>

#include "ers/TabOutStream.h"
#include "ers/Precondition.h"

ers::TabOutStream::TabOutStream(const char* filename) : ers::STLOutStream(filename) {
} // TabOutStream

ers::TabOutStream::TabOutStream() : ers::STLOutStream() {
} // TabOutStream

void ers::TabOutStream::serialize(const std::string &key, const std::string &value) {
    ERS_PRECONDITION(m_stream!=0,"Null STL stream pointer");
    *m_stream  << key << "\t\"" << value << "\"" << std::endl ; 
} // serialize 

void ers::TabOutStream::serialize_start(const Issue *i) {
    ERS_PRECONDITION(m_stream,"Null STL stream pointer");
} // serialize_start

void ers::TabOutStream::serialize_end(const Issue *i) {
    ERS_PRECONDITION(m_stream,"Null STL stream pointer");
    const std::exception *cause = i->cause() ; 
    if (cause) {
	printf("%s %p\n",cause->what(),&cause); 
	*m_stream << Issue::CAUSE_PSEUDO_KEY << "\t\"" << cause->what() << "\"" << std::endl ; 
    } // cause
} // serialize_end


