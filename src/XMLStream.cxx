/*
 *  XMLOutStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <sstream>
#include <assert.h>

#include "ers/XMLStream.h"
#include "ers/Precondition.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/ParseIssue.h"

#include "ers/tinyxml.h"


const char *ers::XMLStream::ISSUE_TAG = "issue" ; 
const char *ers::XMLStream::KEY_TAG = "key" ; 
const char *ers::XMLStream::STRING_VALUE_TAG = "string" ;

ers::XMLStream::XMLStream(const char* filename) {
    m_file_name = filename; 
} // 

ers::XMLStream::~XMLStream() {} // 

void ers::XMLStream::send(const ers::Issue *i) {
    ERS_PRE_CHECK_PTR(i); 
    TiXmlElement issue_item(ISSUE_TAG);
    TiXmlComment comment ; 
    comment.SetValue("Streamed by ers::XMLStream"); 
    issue_item.InsertEndChild(comment); 
    const string_map_type *table = i->get_value_table(); 
    for(string_map_type::const_iterator pos = table->begin();pos!=table->end();++pos) {
	TiXmlElement key_element(KEY_TAG) ;
	TiXmlText key_text(pos->first); 
	key_element.InsertEndChild(key_text); 
	TiXmlElement value_element(STRING_VALUE_TAG) ;
	TiXmlText value_text(pos->second); 
	value_element.InsertEndChild(value_text); 
	issue_item.InsertEndChild(key_element);
	issue_item.InsertEndChild(value_element); 
    } // for
    TiXmlDocument document(m_file_name); 
    document.InsertEndChild(issue_item);
    document.SaveFile(); 
} // send

std::string ers::XMLStream::get_text(const TiXmlElement *element) const {
    ERS_PRE_CHECK_PTR(element); 
    const TiXmlNode* content = element->FirstChild() ; 
    if (!content) { throw ERS_PARSE_ERROR("Element %s has no child",element->Value()); }
    const TiXmlText *text = content->ToText() ; 
    if (! text) { throw ERS_PARSE_ERROR("Element %s has no text chiled",element->Value()); }
    return std::string(text->Value()); 
} // get_text

void ers::XMLStream::cannot_parse(const TiXmlNode *node) const {
    std::ostringstream element_stream ; 
    element_stream << node ; 
    throw ERS_PARSE_ERROR("Cannot parse: %s",element_stream.str().c_str()); 
} // cannot_parse


void ers::XMLStream::parse(const TiXmlElement *element, std::string &key, string_map_type &values) const {
    ERS_PRE_CHECK_PTR(element); 
    const char *v = element->Value() ; 
    if (strcmp(v,KEY_TAG)==0) {
	key = get_text(element); 
	return ; 
    } // KEY_TAG
    if (strcmp(v,STRING_VALUE_TAG)==0) {
	values[key] = get_text(element); 
	return ; 
    } // STRING_VALUE_TAG
    throw ERS_PARSE_ERROR("Unknown tag type: %s",v); 
} // parse

void ers::XMLStream::parse(const TiXmlNode *node, std::string &key, string_map_type &values) const {
    ERS_PRE_CHECK_PTR(node); 
    switch(node->Type()) {
	case TiXmlNode::ELEMENT: 
	    parse(node->ToElement(),key,values);
	    break ; 
	case TiXmlNode::COMMENT:
	    ERS_DEBUG_3("Parsed comment %s",node->Value());
	    break ; 
	default:
	    cannot_parse(node); 
    } // switch
} // parse

ers::Issue *ers::XMLStream::receive()  {
    TiXmlDocument document(m_file_name); 
    bool status = document.LoadFile();
    if (! status) return 0 ; 
    const TiXmlNode *issue_item = document.FirstChild();
    if (! issue_item) return 0 ; 
    if (issue_item->Type()!=TiXmlNode::ELEMENT) { cannot_parse(issue_item); }
    string_map_type values ;
    std::string key ;
    TiXmlNode *child = 0 ; 
    while( child = issue_item->IterateChildren( child ) ) {
	ERS_ASSERT(child!=0,"null child");
	parse(child,key,values); 
    } // while
    std::string class_name = values[Issue::CLASS_KEY] ; 
    Issue *i = ers::IssueFactory::instance()->build(class_name,&values); 
    return i ; 

} // receive
 
