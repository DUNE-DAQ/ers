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
#include "ers/InvalidReferenceIssue.h"


ers::TabInStream::TabInStream(const char* filename) : ers::STLInStream(filename) {} 
ers::TabInStream::TabInStream() : ers::STLInStream() {} 

/** This method parses the input stream and extracts key - values pairs from it
  * The parsing is done line after line, each line is supposed to contain a key, 
  * a tab character, a opening quote, the value and a close quote followed by a carriage return
  * \param key reference to the string where the key is stored 
  * \param value reference to the string where the value is stored 
  * \return \t true if a value was parsed \t false if no line could be read in the file
  * \pre the internal stream pointer is non null
  * \exception ers::Precondition if preconditions are not met 
  */

bool ers::TabInStream::read_line(std::string &key, std::string &value) {
    ERS_PRE_CHECK_PTR(m_stream);
    try {
	std::string line ;
	bool read_status = std::getline(*m_stream,line);
	if (! read_status) return false ; 
	const int line_len = line.size() ; 
	if (line_len< 1) return false ; 
	const unsigned int tab_pos = line.find('\t');
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

/** This method parses the stream and put the resulting key and values into the table passed as parameter. 
  * \param values the table where to put the data
  * \return \t true if some data could be read \t false in the contrary.
  * \pre the internal stream pointer is nott null
  * \pre the \t values parameter is not null 
  * \exception ers::Precondition if preconditions are not met 
  */

bool ers::TabInStream::read_properties(string_map_type *values) {
    ERS_PRE_CHECK_PTR(values);
    bool done_something = false ; 
    while (true) {
	std::string key ; 
	std::string value ; 
	bool status = this->read_line(key,value);
	if (! status) break ;  
	(*values)[key] = value ; 
	done_something = true ; 
    } // while
    return done_something ; 
} // read_properties


