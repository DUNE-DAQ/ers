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
#include "ers/InvalidReferenceIssue.h"


using namespace ers ; 

HumanStream::HumanStream() : STLOutStream(new std::ostringstream()) {
    m_delete_stream = true ;
} // HumanStream

/** Constructor sets up the string-stream 
  */

std::string HumanStream::str() {
    ERS_PRE_CHECK_PTR(m_stream); 
    std::ostringstream *stream = dynamic_cast<std::ostringstream *>(m_stream) ; 
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

void HumanStream::serialize_start(const Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_stream); 
    const std::string message_str = issue->get_value(Issue::MESSAGE_KEY) ;
    const std::string severity_str = issue->get_value(Issue::SEVERITY_KEY) ;
    (*m_stream) << "issue: " << message_str << "(" << severity_str << ")[" ; 
} // serialize_start

void HumanStream::serialize_end(const Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_stream);
    (*m_stream) << "]" ; 
} // serialize_end

void HumanStream::serialize_separator(const Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_stream);
    (*m_stream) << ", " ; 
} // serialize_separator

