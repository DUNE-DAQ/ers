/*
 *  FilterStream.cxx
 *  Working
 *
 *  Created by Matthias Wiesmann on 31.03.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/FilterStream.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/StreamFactory.h"

const char* const ers::FilterStream::FILTER_STREAM_TAG = "filter" ; 


namespace {
    ers::Stream *create_stream(const std::string &protocol, const std::string &uri) { 
	if (protocol==ers::FilterStream::FILTER_STREAM_TAG) {
	    ers::Stream *stream = ers::FilterStream::factory(uri);
	    // ERS_DEBUG_0("build stream %x",stream); 
	    return stream ; 
	} // if
	return 0 ;
    } // create_stream
    bool registered = ers::StreamFactory::instance()->register_factory(ers::Stream::NULL_STREAM_KEY,create_stream) ;
} // anonymous namespace 

ers::FilterStream * ers::FilterStream::factory(const std::string &include_str, const::std::string &exclude_str, const std::string &target_str) {
    ers::Stream *target_stream = ers::StreamFactory::instance()->create_stream(target_str); 
    std::vector<std::string> include_list = ers::Core::tokenize(include_str,",");
    std::vector<std::string> exclude_list = ers::Core::tokenize(exclude_str,","); 
    return new FilterStream(target_stream,include_list,exclude_list); 
} // factory

ers::FilterStream * ers::FilterStream::factory(const std::string &params) {
    std::string::size_type include_start = params.find('?'); 
    std::string::size_type exclude_start = params.find('!');
    std::string::size_type target_start =  params.find('@');
    std::string include_str = params.substr(include_start+1,exclude_start-1);
    std::string exclude_str = params.substr(exclude_start+1,target_start-exclude_start-1);
    std::string target_str = params.substr(target_start+1); 
    // ERS_DEBUG_0("include: '%s' exclude: '%s' target '%s'",include_str.c_str(),exclude_str.c_str(), target_str.c_str()); 
    return factory(include_str,exclude_str,target_str); 
} // factory


ers::FilterStream::FilterStream(ers::Stream *target_ptr, const std::vector<std::string> & include_list,  const std::vector<std::string> & exclude_list) {
    ERS_CHECK_PTR(target_ptr); 
    m_target_stream_ptr = target_ptr ; 
    m_include = include_list ; 
    m_exclude = exclude_list ; 
} // FilterStream

ers::FilterStream::~FilterStream() {
    delete(m_target_stream_ptr) ; 
} // FilterStream


bool ers::FilterStream::is_accept(const ers::Issue* issue_ptr) {
    ERS_CHECK_PTR(issue_ptr);
    std::string qualifiers = issue_ptr->get_value(ers::Issue::QUALIFIER_LIST_KEY) ;
    if (! qualifiers.empty()) { // we check if qualifiers are in exclude list
	std::vector<std::string>::const_iterator e_pos ; 
	for(e_pos = m_exclude.begin();e_pos!=m_exclude.end();e_pos++) {
	    std::string::size_type conflict = qualifiers.find(*e_pos) ; 
	    if (conflict!=std::string::npos) {
		return false ;   
	    } // found conflict 
	} // for 
    } // there are some qualifiers 
    if (! m_include.empty()) {
	std::vector<std::string>::const_iterator i_pos ; 
	for(i_pos = m_include.begin();i_pos!=m_include.end();i_pos++) {
	    std::string::size_type match = qualifiers.find(*i_pos) ; 
	    if (match!=std::string::npos) {
		return true ; 
	    } // found match 
	} // for
	return false ; 
    } // include list contains items 
    return true ; 
} // is_accept



void ers::FilterStream::send(const ers::Issue *issue_ptr) {
    ERS_CHECK_PTR(issue_ptr);
    ERS_CHECK_PTR(m_target_stream_ptr); 
    if (is_accept(issue_ptr)) {
	// ERS_DEBUG_3("accepted %s:",issue_ptr->what()); 
	m_target_stream_ptr->send(issue_ptr); 
    } else {
	// ERS_DEBUG_3("rejected %s:",issue_ptr->what()); 
    } 
} // send