/*
 *  HumanStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <assert.h>
#include "ers/ers.h"
#include "ers/HumanStream.h"

const char* const ers::HumanStream::TXT_SUFFIX = "txt" ; 

namespace {
    ers::Stream *create_stream(const std::string &protocol, const std::string &uri) { 
	if (protocol==System::File::FILE_PROTOCOL) {
	    System::File file(uri); 
	    std::string extension = file.extension(uri) ;
	    if (extension==ers::HumanStream::TXT_SUFFIX) return new ers::HumanStream(file,false); 
	} // tab file
	if ((protocol==ers::STLStream::CERR_STREAM_KEY) && (uri==ers::HumanStream::TXT_SUFFIX)) {
	    return new ers::HumanStream(&std::cerr); 
	} // tab stream
	return 0 ;
    } // 
    bool registered = ers::StreamFactory::instance()->register_factory(ers::HumanStream::TXT_SUFFIX,create_stream) ;
} // anonymous namespace

/** Dumps the content of an issue into a string 
  * \param issue_ptr the Issue to serialise
  * \return string description 
  */

std::string ers::HumanStream::to_string(const Issue *issue_ptr) throw() {
    try {
	std::ostringstream stl_stream ; 
	HumanStream ers_stream(&stl_stream);
	ers_stream.send(issue_ptr) ; 
	std::string str = stl_stream.str();
	return str ; 
    } catch (std::exception &ex) {
	std::string msg = "error while building message " + std::string(ex.what()) ; 
	return msg; 
    } // 
} // to_string

ers::HumanStream::HumanStream() : ers::STLStream() {
} // HumanStream

ers::HumanStream::HumanStream(std::ostream *s) : ers::STLStream(s) {
} // HumanStream

ers::HumanStream::HumanStream(const System::File &file, bool read_mode) : ers::STLStream(file,read_mode) {} 


/** Serializes a key-value pair. 
  * \param issue pointer to the issue owning the pair
  * \param key the key
  * \param value the value
  */

void ers::HumanStream::serialize_pair(const ers::Issue *issue, const std::string &key, const std::string &value) {
    (*m_out_stream)  << key << "=\"" << value << '\"';
} // serialize

/** Starts serialisation of an issue
  * \param issue the issue that is serialised 
  */

void ers::HumanStream::serialize_start(const ers::Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_out_stream); 
    const std::string message_str = issue->get_value(Issue::MESSAGE_KEY) ;
    const std::string severity_str = issue->get_value(Issue::SEVERITY_KEY) ;
    (*m_out_stream) << "issue: " << message_str << "(" << severity_str << ")[" ; 
} // serialize_start

/** Ends serialisation of an issue 
  * \param issue the issue that is serialised 
  */

void ers::HumanStream::serialize_end(const ers::Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_out_stream);
    (*m_out_stream) << "]" ; 
} // serialize_end

/** Serialises a separatar between key-value pairs 
  * \param issue the issue that is serialised 
  */

void ers::HumanStream::serialize_separator(const ers::Issue *issue) {
    ERS_PRE_CHECK_PTR(issue); 
    ERS_PRE_CHECK_PTR(m_out_stream);
    (*m_out_stream) << ", " ; 
} // serialize_separator

