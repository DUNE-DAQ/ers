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

const char* HumanStream::TXT_SUFFIX = "txt" ; 

std::string HumanStream::to_string(const Issue *issue_ptr) {
    std::ostringstream stl_stream ; 
    HumanStream ers_stream(&stl_stream);
    ers_stream.send(issue_ptr) ; 
    std::string str = stl_stream.str();
    return str ; 
} // to_string


HumanStream::HumanStream() : STLStream() {
} // HumanStream

HumanStream::HumanStream(std::ostream *s) : STLStream(s) {
} // HumanStream

HumanStream::HumanStream(const System::File &file, bool read_mode) : STLStream(file,read_mode) {} 


/** Serializes a key-value pair. 
  * \param issue pointer to the issue owning the pair
  * \param key the key
  * \param value the value
  */

void HumanStream::serialize_pair(const Issue *issue, const std::string &key, const std::string &value) {
    (*m_out_stream)  << key << "=\"" << value << '\"';
} // serialize

/** Starts serialisation of an issue
  * \param issue the issue that is serialised 
  */

void HumanStream::serialize_start(const Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_out_stream); 
    const std::string message_str = issue->get_value(Issue::MESSAGE_KEY) ;
    const std::string severity_str = issue->get_value(Issue::SEVERITY_KEY) ;
    (*m_out_stream) << "issue: " << message_str << "(" << severity_str << ")[" ; 
} // serialize_start

/** Ends serialisation of an issue 
  * \param issue the issue that is serialised 
  */

void HumanStream::serialize_end(const Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_out_stream);
    (*m_out_stream) << "]" ; 
} // serialize_end

/** Serialises a separatar between key-value pairs 
  * \param issue the issue that is serialised 
  */

void HumanStream::serialize_separator(const Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_out_stream);
    (*m_out_stream) << ", " ; 
} // serialize_separator

