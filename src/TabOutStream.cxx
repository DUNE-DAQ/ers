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
    ERS_PRECONDITION(_stream,"Null STL stream pointer");
    *_stream  << key << "\t\"" << value << "\"" << std::endl ; 
} // serialize 

void ers::TabOutStream::serialize_start(const Issue *i) {
    ERS_PRECONDITION(_stream,"Null STL stream pointer");
} // serialize_start

void ers::TabOutStream::serialize_end(const Issue *i) {
    ERS_PRECONDITION(_stream,"Null STL stream pointer");
} // serialize_end


