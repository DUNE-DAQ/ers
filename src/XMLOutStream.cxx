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

const char *ers::XMLOutStream::ISSUE_TAG = "issue" ; 
const char *ers::XMLOutStream::KEY_TAG = "key" ; 
const char *ers::XMLOutStream::STRING_VALUE_TAG = "string" ;

ers::XMLOutStream::XMLOutStream(const char* filename) : ers::STLOutStream(filename) {
} // XML_Stream

ers::XMLOutStream::XMLOutStream() : ers::STLOutStream() {
 } // XML_Stream

void ers::XMLOutStream::tag_open(const std::string &tag) {
    ERS_PRECONDITION(m_stream,"Null STL stream pointer");
    *m_stream << "<" << tag << ">" ; 
} // tag_open

void ers::XMLOutStream::tag_close(const std::string &tag) {
    ERS_PRECONDITION(m_stream,"Null STL stream pointer");
    *m_stream << "</" << tag << ">" ; 
} // tag_close

void ers::XMLOutStream::tag(const std::string &tag, const std::string &content) {
    ERS_PRECONDITION(m_stream,"Null STL stream pointer");
    tag_open(tag);
    *m_stream << content  ;
    tag_close(tag); 
} // tag

void ers::XMLOutStream::serialize(const std::string &key, const std::string &value) {
    ERS_PRECONDITION(m_stream,"Null STL stream pointer");
    tag(KEY_TAG,key);
    tag(STRING_VALUE_TAG,value); 
    *m_stream << std::endl ; 
} // serialize 

void ers::XMLOutStream::serialize_start(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(m_stream!=0,"Null STL stream pointer");
    tag_open(ISSUE_TAG);
} // serialize_start

void ers::XMLOutStream::serialize_end(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue"); 
    ERS_PRECONDITION(m_stream!=0,"Null STL stream pointer");
    const std::exception *cause = i->cause(); 
    if (cause) {
	tag_open(ers::Issue::CAUSE_PSEUDO_KEY);
	if (typeid(i) == typeid(const ers::Issue *)) {
	    *m_stream << std::endl ; 
	    const Issue *cause_issue = dynamic_cast<const ers::Issue *> (cause) ; 
	    this->send(cause_issue); 
	} else {
	    std::string cause_value = cause->what() ; 
	    tag(STRING_VALUE_TAG,cause_value) ; 
	    *m_stream << std::endl ; 
	}
	tag_open(ers::Issue::CAUSE_PSEUDO_KEY);
    } // 
    *m_stream << "</issue>" << std::endl ; 
} // serialize_end

