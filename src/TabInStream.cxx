/*
 *  TabInStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 17.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <string>
#include <istream>
#include "ers/TabInStream.h"
#include "ers/Precondition.h"
#include "ers/ParseIssue.h"

ers::TabInStream::TabInStream(const char* filename) : ers::STLInStream(filename) {} 
ers::TabInStream::TabInStream() : ers::STLInStream() {} 

bool ers::TabInStream::read_line(std::string &key, std::string &value) {
    ERS_PRECONDITION(m_stream,"null stream");
    try {
	std::string line ;
	bool read_status = std::getline(*m_stream,line);
	if (! read_status) return false ; 
	const int line_len = line.size() ; 
	if (line_len< 1) return false ; 
	const int tab_pos = line.find('\t');
	if (tab_pos == std::string::npos) {
	    throw ERS_PARSE_ERROR("Parse error - could not find tab",line); 
	} // no tab found 
	key = line.substr(0,tab_pos) ;
	const int value_len = line_len - tab_pos - 3 ; 
	value = line.substr(tab_pos+2,value_len) ; // we have to remove the tab + quotes
	return true ;
    } catch (std::ios_base::failure &ex) {
	if (m_stream->eof()) return false ; 
	throw ; 
    } 
} // read_line

bool ers::TabInStream::read_properties(string_map_type *values) {
    ERS_PRECONDITION(values,"null value table");
    while (true) {
	std::string key ; 
	std::string value ; 
	bool status = this->read_line(key,value);
	if (! status) break ;  
	(*values)[key] = value ; 
    } // while
    return true ; 
} // read_properties


