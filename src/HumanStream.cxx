/*
 *  HumanStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <assert.h>
#include "ers/HumanStream.h"
#include "ers/Issue.h"
#include "ers/Precondition.h"

using namespace ers ; 

HumanStream::HumanStream() : STLOutStream(new std::ostringstream()) {
    m_delete_stream = true ;
} // HumanStream

/** Constructor sets up the string-stream 
  */

std::string HumanStream::str() {
    std::ostringstream *stream = dynamic_cast<std::ostringstream *>(this->m_stream) ; 
    std::string s = stream->str();
    return s ; 
} // str

/** Serializes a key-value pair. 
  * @param key the key
  * @param value the value
  */

void HumanStream::serialize(const std::string &key, const std::string &value) {
    (*m_stream)  << key << "=\"" << value << '\"';
} // serialize

void HumanStream::serialize_start(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(*m_stream,"Null STL stream pointer"); 
    const std::string message_str = i->get_value(Issue::MESSAGE_KEY) ;
    const std::string severity_str = i->get_value(Issue::SEVERITY_KEY) ;
    (*m_stream) << "issue: " << message_str << "(" << severity_str << ")[" ; 
} // serialize_start

void HumanStream::serialize_end(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(*m_stream,"Null STL stream pointer"); 
    (*m_stream) << "]" ; 
} // serialize_end

void HumanStream::serialize_separator(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(*m_stream,"Null STL stream pointer"); 
    (*m_stream) << ", " ; 
} // serialize_separator

