/*
 *  TabStream.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 28.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/TabStream.h"
#include "ers/ers.h"
#include "ers/ParseIssue.h"

const char* ers::TabStream::TAB_SUFFIX = "tab" ;     

ers::TabStream::TabStream() : STLStream() {} 
ers::TabStream::TabStream(const System::File &file, bool read_mode) : STLStream(file,read_mode) {}
ers::TabStream::TabStream(std::istream *in_stream) : STLStream(in_stream) {} 
ers::TabStream::TabStream(std::ostream *out_stream) : STLStream(out_stream) {}
ers::TabStream::~TabStream() {}


/** This method parses the input stream and extracts key - values pairs from it
* The parsing is done line after line, each line is supposed to contain a key, 
* a tab character, a opening quote, the value and a close quote followed by a carriage return
* \param key reference to the string where the key is stored 
* \param value reference to the string where the value is stored 
* \return \c true if a value was parsed \c false if no line could be read in the file
* \pre the internal stream pointer is non null
* \exception ers::Precondition if preconditions are not met 
*/

bool ers::TabStream::read_line(std::string &key, std::string &value) {
    ERS_PRE_CHECK_PTR(m_in_stream);
    try {
	std::string line ;
	bool read_status = std::getline(*m_in_stream,line);
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
	if (m_in_stream->eof()) return false ; 
	throw ; 
    } // catch 
} // read_line

/** This method parses the stream and put the resulting key and values into the table passed as parameter. 
 * \param values the table where to put the data
 * \return ct true if some data could be read ct false in the contrary.
 * \pre the internal stream pointer is nott null
 * \pre the \c values parameter is not null 
 * \exception ers::InvalidReference if preconditions are not met 
 */

bool ers::TabStream::deserialize(string_map_type *values) {
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

/** Serializes a key-value pair. 
* This is done by: 
* - printing the key
* - printing a tabulation character
* - printing a opening quote "
* - printing the value 
* - printing a closing quote "
* - printing a carriage return
* \param issue_ptr pointer to the issue
* \param key the key to serialize
* \param value the value to serialize
*/

void ers::TabStream::serialize_pair(const Issue *issue_ptr, const std::string &key, const std::string &value) {
    ERS_PRE_CHECK_PTR(m_out_stream);
    *m_out_stream  << key << "\t\"" << value << "\"" << std::endl ; 
} // serialize 

/** Finishes the serialization
* As this serialization method does not support for serializing causes, 
* it adds a pseudo key with the description of the cause. 
* \param i the issue to serialize
* \bug at the moment cause exceptions are not supported, and the root exception adds the cause property by itself
*/

void ers::TabStream::serialize_end(const Issue *i) {
    ERS_PRE_CHECK_PTR(m_out_stream);
    *m_out_stream << std::endl ; 
} // serialize_end

