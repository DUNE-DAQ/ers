/*
 *  XercesStream.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 12.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <sstream>
#include <iostream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>

#include "ers/XercesStream.h"
#include "ers/XercesString.h"
#include "ers/Precondition.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/ParseIssue.h"
#include "ers/NotImplemented.h"

static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };



ers::XercesStream::XercesStream(const System::File &file) : Stream() ,DOMErrorHandler() {
    m_file_path = file.full_name(); 
} // XercesStream

ers::XercesStream::~XercesStream() {
    
} //~XercesStream


// Error Handling methods
// ----------------------

std::string ers::XercesStream::error_msg(const DOMNode *node) {
    const XMLCh* data = node->getNodeValue(); 
    const XMLCh* name = node->getNodeName() ; 
    std::ostringstream element_stream ; 
    element_stream <<  to_string(name) << ":" << to_string(data)  ; 
    return element_stream.str() ; 
} // error_msg

ers::Issue ers::XercesStream::to_issue(const DOMError& domError){
    const short x_severity = domError.getSeverity();
    ers::ers_severity severity = ers_error ;
    switch (x_severity) {
	case DOMError::DOM_SEVERITY_WARNING: severity = ers_warning ; break ;
	case DOMError::DOM_SEVERITY_ERROR: severity = ers_error ; break ; 
	case DOMError::DOM_SEVERITY_FATAL_ERROR: severity = ers_fatal ; break ; 
	default: 
	    NOT_IMPLEMENTED(); 
	    break ; 
    } // switch 
    std::string message = to_string(domError.getMessage()); 
    ParseIssue issue(ERS_HERE,severity,message);
    DOMLocator *location = domError.getLocation(); 
    const int line_number = location->getLineNumber() ;
    issue.offending_line_number(line_number); 
    std::string uri = to_string(location->getURI());
    issue.file_name(uri); 
    DOMNode *node = location->getErrorNode(); 
    std::string node_name = to_string(node->getNodeName()) + to_string(node->getNodeValue()) ; 
    issue.offending_line(node_name);
    return issue ; 
} // 


void ers::XercesStream::cannot_parse(const DOMNode *node) const {
    throw ERS_PARSE_ERROR("Cannot parse element '%s'",error_msg(node).c_str()); 
} // cannot_parse

bool ers::XercesStream::handleError(const DOMError& domError) {
    Issue issue = to_issue(domError); 
    dispatch(&issue,true); 
    return true  ; 
} //

/** Extracts the text part of a node 
  * \param node the node to extract text from 
  * \return a string with the text part, an empty string if no text is available 
  */

std::string ers::XercesStream::get_text(const DOMNode *node) {
    const XMLCh* data = node->getNodeValue(); 
    if (data) return to_string(data); 
    return "" ; 
} // get_text

/** Extracts the text for an element.
  * This is the text of the first child of the element. 
  * I.e this method assumes that the element does not contain further elements 
  * \param element the element to extract the text from 
  * \return a string containing the text
  */

std::string ers::XercesStream::get_text(const DOMElement *element) {
    const DOMNode* child = element->getFirstChild() ; 
    return get_text(child); 
}  // get_text

/** Parses an element (i.e a tag like \<key\>)
  * \li If the element is a key tag, its content is taken as a key name and put into parameter \c key
  * \li If the element is a string tag, its content is taken as a value, and it is put with parameter \c key
  * Into the \c values. 
  * \param the element to parse
  * \param key the last key 
  * \values the table containing all key and values
  */

void ers::XercesStream::parse(const DOMElement *element, std::string &key, string_map_type &values) const {
    ERS_PRE_CHECK_PTR(element); 
    const XMLCh *x_tag = element->getTagName() ;  
    std::string tag = to_string(x_tag); 
    if (tag == XML_KEY_TAG) {
	key = get_text(element); 
	return ; 
    } 
    if (tag == XML_STRING_VALUE_TAG) {
	values[key] = get_text(element);
	return ;
    } 
    throw ERS_PARSE_ERROR("Unknown tag type: %s",tag); 
} // parse

void ers::XercesStream::parse(const DOMNode *node, std::string &key, string_map_type &values) const {
    ERS_PRE_CHECK_PTR(node); 
    if (node->getNodeType()==DOMNode::ELEMENT_NODE) {
	const DOMElement *element = dynamic_cast <const DOMElement *> (node) ;
	parse(element,key,values); 
	return ; 
    } // Element node
    if (node->getNodeType()==DOMNode::TEXT_NODE) {
	return ; 
    } // TEXT node
    if (node->getNodeType()==DOMNode::COMMENT_NODE) {
	std::string text = get_text(node) ; 
	// std::cerr << "parse comment \"" << text << "\"" << std::endl ;  
	return ; 
    } // TEXT node
    cannot_parse(node); 
} // parse

ers::Issue *ers::XercesStream::receive(const DOMElement *issue_element_ptr) const {
    string_map_type values ;
    std::string key ;
    DOMNode *child = issue_element_ptr->getFirstChild() ; 
    while(child!=0) {
	parse(child,key,values); 
	child=child->getNextSibling() ; 
    } //
    std::string class_name = values[Issue::CLASS_KEY] ; 
    Issue *issue_ptr = ers::IssueFactory::instance()->build(class_name,&values); 
    return issue_ptr ;
} // receive

ers::Issue *ers::XercesStream::receive(const DOMDocument *document_ptr) const {
    ERS_PRE_CHECK_PTR(document_ptr);
    const DOMNode *issue_node = document_ptr->getFirstChild();
    if (! issue_node) return 0 ;     
    if (issue_node->getNodeType()!=DOMNode::ELEMENT_NODE) { cannot_parse(issue_node); }
    std::cerr << "Receiving document" << std::endl ; 
    const DOMElement *issue_element = dynamic_cast <const DOMElement *> (issue_node) ;
    ERS_PRE_CHECK_PTR(issue_element);
    std::string issue_name = to_string(issue_element->getTagName()) ; 
    if (issue_name!=XML_ISSUE_TAG) throw ERS_PARSE_ERROR("Invalid root tag : %s (should be %s)",issue_name,XML_ISSUE_TAG); 
    return receive(issue_element);
} // receive

ers::Issue *ers::XercesStream::receive() {
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
    DOMBuilder        *parser = ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
    parser->setFeature(XMLUni::fgDOMNamespaces, false);
    parser->setFeature(XMLUni::fgXercesSchema, false);
    parser->setFeature(XMLUni::fgDOMValidation, false);
    parser->setErrorHandler((DOMErrorHandler *) this) ; 
    parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);
    parser->resetDocumentPool();
    std::cerr << "Parsing document " << std::endl ; 
    DOMDocument *document = parser->parseURI(m_file_path.c_str()); 
    std::cerr << "Parsing done " << std::endl ; 
    if (document) {
	return receive(document); 
    } else {
	return 0 ;
    } // else 
} // receive

void ers::XercesStream::send(DOMDocument *document_ptr, const Issue *issue_ptr) {
    DOMElement *root_element = document_ptr->getDocumentElement();
    const string_map_type *table = issue_ptr->get_value_table(); 
    for(string_map_type::const_iterator pos = table->begin();pos!=table->end();++pos) {
	DOMElement *key_element = document_ptr->createElement(to_unicode(ers::XML_KEY_TAG)) ; 
	DOMText *key_text = document_ptr->createTextNode(to_unicode(pos->first.c_str())) ; 
	key_element->appendChild((DOMNode *)key_text);
	root_element->appendChild(key_element);
	DOMElement *value_element = document_ptr->createElement(to_unicode(ers::XML_STRING_VALUE_TAG)) ; 
	DOMText *value_text = document_ptr->createTextNode(to_unicode(pos->second.c_str())) ;
	value_element->appendChild((DOMNode *) value_text);
	root_element->appendChild(value_element);
    } // for 
} // send

void ers::XercesStream::send(const Issue *issue_ptr) {
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
    DOMDocument* document_ptr = impl->createDocument(0,to_unicode(XML_ISSUE_TAG),0);
    document_ptr->setStandalone(true); 
    send(document_ptr,issue_ptr);  
    XMLFormatTarget *form_target = new LocalFileFormatTarget(m_file_path.c_str());
    DOMWriter *writer_ptr = impl->createDOMWriter();
    writer_ptr->setFeature(to_unicode("format-pretty-print"),true) ; 
    writer_ptr->writeNode(form_target,*document_ptr);
    writer_ptr->release(); 
    document_ptr->release();
    delete form_target ; 
}// send



