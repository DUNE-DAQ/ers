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
#include "ers/InvalidReferenceIssue.h"

ers::TabOutStream::TabOutStream() : ers::STLOutStream() {} 
ers::TabOutStream::TabOutStream(const char* filename) : ers::STLOutStream(filename) {} 
ers::TabOutStream::TabOutStream(std::ostream *s) : ers::STLOutStream(s) {} 
ers::TabOutStream::TabOutStream(const std::string & filename) : ers::STLOutStream(filename) {} 
ers::TabOutStream::TabOutStream(const System::File & file) : ers::STLOutStream(file) {} 

/** Serializes a key-value pair. 
  * This is done by: 
  * - printing the key
  * - printing a tabulation character
  * - printing a opening quote "
  * - printing the value 
  * - printing a closing quote "
  * - printing a carriage return
  * \param key the key to serialize
  * \param value the value to serialize
  */

void ers::TabOutStream::serialize(const std::string &key, const std::string &value) {
    ERS_PRE_CHECK_PTR(m_stream);
    *m_stream  << key << "\t\"" << value << "\"" << std::endl ; 
} // serialize 

void ers::TabOutStream::serialize_start(const Issue *i) {
    ERS_PRE_CHECK_PTR(m_stream);
} // serialize_start

/** Finishes the serialization
  * As this serialization method does not support for serializing causes, 
  * it adds a pseudo key with the description of the cause. 
  * \param i the issue to serialize
  * \bug at the moment cause exceptions are not supported, and the root exception adds the cause property by itself
  */

void ers::TabOutStream::serialize_end(const Issue *i) {
    ERS_PRE_CHECK_PTR(m_stream);
    *m_stream << std::endl ; 
} // serialize_end


