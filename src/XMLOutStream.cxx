/*
 *  XMLOutStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <assert.h>

#include "ers/XMLOutStream.h"
#include "ers/Precondition.h"

ers::XMLOutStream::XMLOutStream(const char* filename) : ers::STLOutStream(filename) {
} // XML_Stream

ers::XMLOutStream::XMLOutStream() : ers::STLOutStream() {
 } // XML_Stream

void ers::XMLOutStream::serialize(const std::string &key, const std::string &value) {
    ERS_PRECONDITION(_stream,"Null STL stream pointer");
    *_stream << "<key>" << key << "</key><string>" << value << "</string>" << std::endl ; 
} // serialize 

void ers::XMLOutStream::serialize_start(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(_stream,"Null STL stream pointer");
    *_stream << "<issue>" << std::endl ; 
} // serialize_start

void ers::XMLOutStream::serialize_end(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(_stream,"Null STL stream pointer");
    *_stream << "</issue>" << std::endl ; 
} // serialize_end

