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
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>


#include "ers/XercesStream.h"
#include "ers/XercesString.h"
#include "ers/ers.h"

#include "ers/ParseIssue.h"

#include "system/IOIssue.h"

const char * const ers::XercesStream::XML_SUFFIX = "xml" ; 
const char * const ers::XercesStream::XML_TAGS[] = { "issue", "key", "string", "issuelist" } ; 

#define XML_ISSUE_TAG ers::XercesStream::XML_TAGS[0] 
#define XML_KEY_TAG ers::XercesStream::XML_TAGS[1] 
#define XML_STRING_VALUE_TAG ers::XercesStream::XML_TAGS[2] 
#define XML_LIST_TAG ers::XercesStream::XML_TAGS[3] 


namespace {
    ers::Stream *create_stream(const std::string &protocol, const std::string &uri) { 
	if (protocol==System::File::FILE_PROTOCOL) {
	    System::File file(uri); 
	    std::string extension = file.extension(uri) ;
	    if (ers::XercesStream::XML_SUFFIX) return new ers::XercesStream(file,false); 
	} // tab file
	if ((protocol==ers::STLStream::CERR_STREAM_KEY) && (uri==ers::XercesStream::XML_SUFFIX)) {
	    return new ers::XercesStream(&std::cerr); 
	} // tab stream
	return 0 ;
    } // 
    bool registered = ers::StreamFactory::instance()->register_factory(ers::XercesStream::XML_SUFFIX,create_stream) ;
} // anonymous namespace


/** Stuff needed to initialise the DOM implementation. 
  * Probably a list off character sets 
  */

static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };

/** Constructor - builds a stream that will write into a file
  * \param file the file data will be written into 
  * \param read_mode is the file used for reading 
  */

ers::XercesStream::XercesStream(const System::File &file, bool read_mode) : STLStream(file,read_mode) ,DOMErrorHandler() {
    m_file_path = file.full_name() ; 
    init(read_mode); 
} // XercesStream

ers::XercesStream::XercesStream(std::ostream *out_stream) : STLStream(out_stream) {
    init(false); 
} // XercesStream

/** Destructor 
  */

ers::XercesStream::~XercesStream() {
    if (m_flush_write) {
	commit_writes();
    } 
} //~XercesStream

/** Internal initialisation method 
  */

void ers::XercesStream::init(bool read_mode) {
    m_flush_write = false ; 
    m_pretty_print = false ;  // putting this to true produces strange empty lines that break parsing 
    m_dom_implementation_ptr = DOMImplementationRegistry::getDOMImplementation(gLS);
    ERS_CHECK_PTR(m_dom_implementation_ptr) ; 
    if (read_mode) {
	m_document_ptr = 0 ; 
    } else {
	m_document_ptr = m_dom_implementation_ptr->createDocument(0,to_unicode(XML_LIST_TAG),0);
	ERS_CHECK_PTR(m_document_ptr); 
	m_document_ptr->setStandalone(true); 
    } 
} // init


// ----------------------
// Error Handling methods
// ----------------------

/** Extracts an error message out of a node
  * \param node pointer to the node to information about
  * \return a string containg the name and the data for the node separated by a colon (:) 
  */

std::string ers::XercesStream::error_msg(const DOMNode *node) {
    if (!node) {
	return "null node" ; 
    } // no node
    const XMLCh* data = node->getNodeValue(); 
    const XMLCh* name = node->getNodeName() ; 
    std::ostringstream element_stream ; 
    element_stream <<  to_string(name) << ":" << to_string(data)  ; 
    return element_stream.str() ; 
} // error_msg

/** Converts a DOM error into an Issue (ParseIssue). 
  * The following conversions are done: 
  * \li The severity is translated (warning becomes a warning and both error and fatal become errors).
  * \li The error message is copied without changes 
  * \li The line number is inserted into the \c ParseIssue using method \c offending_line_number() 
  * \li The name of the file is inserted into the \c ParseIssue using method \c file_name()
  * \li The problematic node is inserted into the \c ParseIssue using method \c offending_line()
  * \param domError the DOM error
  * \return An Issue object with all the relevant fields set up 
  */

ers::Issue ers::XercesStream::to_issue(const DOMError& domError){
    const short x_severity = domError.getSeverity();
    ers::ers_severity severity = ers_error ;
    switch (x_severity) {
	case DOMError::DOM_SEVERITY_WARNING: severity = ers_warning ; break ;
	case DOMError::DOM_SEVERITY_ERROR: severity = ers_error ; break ; 
	case DOMError::DOM_SEVERITY_FATAL_ERROR: severity = ers_error ; break ; 
	default: 
	    ERS_NOT_IMPLEMENTED(); 
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
    std::string node_name = error_msg(node); 
    issue.offending_line(node_name);
    return issue ; 
} // 

/** This method is called when a node cannot be parsed.
  * It throws an Issue with the relevant fields set up 
  * \param node the node that cannot be parsed
  */

void ers::XercesStream::cannot_parse(const DOMNode *node) const {
    throw ERS_PARSE_ERROR("Cannot parse element '%s'",error_msg(node).c_str()); 
} // cannot_parse

/** DOM Error handling method - this method is called when the DOM parser encounters an error.
  * If the error is a warning, it is send to the appropriate stream. 
  * If it is an error, the appropriate Issue is build and thrown as an exception 
  * \param domError the actual error encoutered 
  * \return either \c true or an exception is thrown 
  */
  
bool ers::XercesStream::handleError(const DOMError& domError) {
    Issue issue = to_issue(domError); 
    StreamFactory::dispatch(&issue,true); 
    return true  ; 
} //

/** Extracts the text part of a node 
  * \param node the node to extract text from 
  * \return a string with the text part, an empty string if no text is available 
  */

std::string ers::XercesStream::get_text(const DOMNode *node) {
    if (0==node) return ers::Core::empty_string ; 
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

/** Parses a node
  * Depending on the type of node, a different behaviour is taken.
  * Elements are parsed further, text and comments are ignored. 
  * This method recognises three types of elements: 
  * \li \c \<key\> the text child of the element is handled as the name of a key and stored into variable \c key
  * \li \c \<string\> the text child of the element is handled as a value, it is inserted into \c values with \c key
  * \li \c \<issue\> the issue is parsed and its address written into \c cause
  * \param node the node to parse 
  * \param key string to use to store a key when recognised 
  * \param values the key/value table used to store values
  * \param cause address to use to store the pointer for cause issues
  * \note if \c XERCES_STREAM_LOG_COMMENT is defined, comments are logged at level 3. 
  * \note if the \c issue tag appears and the key is to set to \c CAUSE_PSEUDO_KEY, an exception is thrown
  *       (this means that the issue was not specified after a key). 
  * \note if multiple values are specified for a key, the last one will be kept. 
  */

void ers::XercesStream::parse(const DOMNode *node, std::string &key, string_map_type &values, const ers::Issue **cause) const {
    ERS_PRE_CHECK_PTR(node); 
    ERS_PRE_CHECK_PTR(cause); 
    if (node->getNodeType()==DOMNode::ELEMENT_NODE) {
	const DOMElement *element = dynamic_cast <const DOMElement *> (node) ;
	// std::cerr << "Parsing " << get_text(element) << std::endl ; 
	const XMLCh *x_tag = element->getTagName() ;  
	std::string tag = to_string(x_tag); 
	if (tag == XML_KEY_TAG) { // We have a key 
	    key = get_text(element); 
	    return ; 
	} else if (tag == XML_STRING_VALUE_TAG) { // we have a string value 
	    values[key] = get_text(element);
	    return ;
	} else if (tag== XML_ISSUE_TAG) { // we have a issue 
	    ERS_ASSERT(key==ers::Issue::CAUSE_PSEUDO_KEY,"key for cause is %s, not %s",key.c_str(),ers::Issue::CAUSE_PSEUDO_KEY);
	    *cause = receive(element) ; 
	} else { // we don't know what we have... 
	    throw ERS_PARSE_ERROR("Unknown tag type: %s",tag); 
	} // default case
    } // Element node
    if (node->getNodeType()==DOMNode::TEXT_NODE) {
	return ; 
    } // TEXT node
    if (node->getNodeType()==DOMNode::COMMENT_NODE) {
#ifdef XERCES_STREAM_LOG_COMMENT
	std::string text = get_text(node) ; 
	ERS_DEBUG_3(text.c_str()) ; 
#endif
	return ; 
    } // Comment node
    cannot_parse(node); 
} // parse

/** Read an Issue out of a root element. 
  * This method assumes that the issue is of the right type (i.e the root \<issue\> tag). 
  * \param issue_element_ptr pointer to the root element 
  * \return an issue if it could be read, or a null pointer if there was no issue in the element 
  */

ers::Issue *ers::XercesStream::receive(const DOMElement *issue_element_ptr) const {
    string_map_type values ;
    std::string key ;
    DOMNode *child = issue_element_ptr->getFirstChild() ; 
    const ers::Issue *cause_ptr = 0 ; 
    while(child!=0) {
	parse(child,key,values,&cause_ptr); 
	child=child->getNextSibling() ; 
    } //
    std::string class_name = values[Issue::CLASS_KEY] ; 
    Issue *issue_ptr = ers::IssueFactory::instance()->build(class_name,&values); 
    if (issue_ptr && cause_ptr) {
	issue_ptr->cause(cause_ptr); 
    } // we have a cause issue
    return issue_ptr ;
} // receive

/** Reads an Issue out of DOM document 
  * \param document_ptr pointer to the root document where the issue should be 
  * \return an issue if it could be read, or a null pointer if there was no issue in the document 
  * \pre \c document_ptr is a valid pointer
  */

ers::Issue *ers::XercesStream::receive(const DOMDocument *document_ptr) const {
    ERS_PRE_CHECK_PTR(document_ptr);
    const DOMNode *root_node = document_ptr->getFirstChild();
    if (! root_node) return 0 ; 
    const DOMNode *issue_node = root_node->getFirstChild(); 
    if (! issue_node) return 0 ;     
    if (issue_node->getNodeType()!=DOMNode::ELEMENT_NODE) { cannot_parse(issue_node); }
    const DOMElement *issue_element = dynamic_cast <const DOMElement *> (issue_node) ;
    ERS_CHECK_PTR(issue_element);
    std::string issue_name = to_string(issue_element->getTagName()) ; 
    if (issue_name!=XML_ISSUE_TAG) {
    	throw ERS_PARSE_ERROR("Invalid root tag : %s (should be %s)",
	issue_name,XML_ISSUE_TAG); 
    } // not issue 
    return receive(issue_element);
} // receive

/** Reads an Issue in the Stream
  * \return an issue if it could be read, or a null pointer if there was no issue in the stream 
  */

ers::Issue *ers::XercesStream::receive() {
    DOMBuilder *parser = ((DOMImplementationLS*) m_dom_implementation_ptr)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
    ERS_CHECK_PTR(parser);
    parser->setFeature(XMLUni::fgDOMNamespaces, false);
    parser->setFeature(XMLUni::fgXercesSchema, false);
    parser->setFeature(XMLUni::fgDOMValidation, false);
    parser->setErrorHandler((DOMErrorHandler *) this) ; 
    // parser->setFeature(XMLUni::fgDOMDatatypeNormalization, false);
    parser->resetDocumentPool();
    
    MemBufInputSource *src = get_source(); 
    // src->setEncoding(to_unicode("utf-8")); 
    Wrapper4InputSource wrapper(src,true); // We need to convert between formats because DOM seems not to support reading from memory.
    DOMDocument *document = parser->parse(wrapper) ; // parseURI(m_file_path.c_str()); 
    // delete(src); 
    if (document) {
	return receive(document); 
    } else {
	return 0 ;
    } // else 
} // receive

/** Copies the content of the stream into a buffer that can be parsed.
  * \return a Memory buffer for SAX/DOM
  * \note Copy is very innefficient, for some wierd reason, readsome throws 
  * exceptions, so reads have to be done one by one. 
  * It would probably be more efficient to address the underlying buffer.
  * Ideally, this stream should only 'consume' xml data for one issue 
  */

MemBufInputSource *ers::XercesStream::get_source() {
    ERS_CHECK_PTR(m_in_stream);
    char buffer[65000] ; 
    char *p = buffer ; 
    int total = 0 ; 
    try { 
	do {
	    const int space_left = sizeof(buffer)-total ; 
	    ERS_ASSERT(space_left>0,"Buffer overflow");
	    int byte_read = 1 ; // = m_in_stream->readsome(p,16); 
	    int c = m_in_stream->get();
	    if (c==EOF) break ;
	    (*p) = c ; 
	    p+=byte_read ; 
	    total+=byte_read ;
	} while(m_in_stream->peek()!=EOF) ; 
	const char *id = m_file_path.c_str();  
	MemBufInputSource* memBufIS = new MemBufInputSource((const XMLByte*) buffer,total,id,false); 
	return memBufIS ; 
    } catch (std::ios_base::failure &e) {
	throw SYSTEM_IOERROR("failure copying buffer from "+m_file_path,&e);
    } 
} // 

/** Adds a simple string tag with the following format
  * \verbatim
  * <name>value</name>
  * \endverbatim
  * \param element_ptr the element where things should be added 
  * \param name the name of the tag (should be a valid tag identifier, i.e no space). 
  * \param value the textual value of the tag 
  * \pre \c element_ptr is a valid pointer
  * \pre \c name is a valid pointer
  * \pre \c value is a valid point
  */

void ers::XercesStream::add_string_tag(DOMElement *element_ptr, const char *name, const char *value) {
    ERS_PRE_CHECK_PTR(element_ptr);
    ERS_PRE_CHECK_PTR(name);
    ERS_PRE_CHECK_PTR(value);
    ERS_PRE_CHECK_PTR(m_document_ptr); 
    DOMElement *child_element = m_document_ptr->createElement(to_unicode(name)) ; 
    DOMText *child_text = m_document_ptr->createTextNode(to_unicode(value)) ; 
    child_element->appendChild((DOMNode *) child_text);
    element_ptr->appendChild(child_element);
} // send

/** This method serializes an issue into a XML element.
  * The method can recusively call itself to serialize cause issues 
  * \param root_element the element in which to insert the issue content (i.e an issue tag).
  * \param issue_ptr the issue to serialize 
  * \pre \c root_element is a valid pointer
  * \pre \c issue_ptr is a valid pointer
  */

void ers::XercesStream::serialize(DOMElement *root_element, const Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr);
    ERS_PRE_CHECK_PTR(m_document_ptr); 
    ERS_CHECK_PTR(root_element);
    DOMElement *issue_element = m_document_ptr->createElement(to_unicode(XML_ISSUE_TAG)) ;
    ERS_CHECK_PTR(issue_element);
    root_element->appendChild(issue_element);
    const string_map_type *table = issue_ptr->get_value_table(); 
    ERS_CHECK_PTR(table); 
    for(string_map_type::const_iterator pos = table->begin();pos!=table->end();++pos) {
	add_string_tag(issue_element,XML_KEY_TAG,pos->first.c_str()) ; 
	add_string_tag(issue_element,XML_STRING_VALUE_TAG,pos->second.c_str()) ; 
    } // for
    const Issue *cause = issue_ptr->cause();
    if (cause) {
	add_string_tag(issue_element,XML_KEY_TAG,ers::Issue::CAUSE_PSEUDO_KEY) ;
	DOMElement *cause_element = m_document_ptr->createElement(to_unicode(XML_KEY_TAG)) ;
	serialize(cause_element,cause);
    } // cause
    
} // send

/** Sends an issue into the stream 
  * This method sets up the document and streams the Issue into it. 
  * \param issue_ptr pointer to the issue to send
  */

void ers::XercesStream::send(const Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr);
    DOMElement *root_element = m_document_ptr->getDocumentElement();
    ERS_CHECK_PTR(root_element); 
    serialize(root_element,issue_ptr); 
    m_flush_write = true ; 
    // send(m_document_ptr,issue_ptr);  
}// send

void ers::XercesStream::commit_writes() {
    XMLFormatTarget *form_target = this ; 
    ERS_CHECK_PTR(form_target);
    DOMWriter *writer_ptr = m_dom_implementation_ptr->createDOMWriter();
    ERS_CHECK_PTR(writer_ptr);
    writer_ptr->setFeature(to_unicode("format-pretty-print"),m_pretty_print) ; 
    writer_ptr->writeNode(form_target,*m_document_ptr);
    writer_ptr->release(); 
    m_document_ptr->release();
    m_document_ptr= 0 ; 
    m_out_stream->flush(); 
} // commit_writes


/** Method required for XML Format target
  */

void ers::XercesStream::writeChars(const XMLByte* const toWrite, const unsigned int count,XMLFormatter* const formatter) {
    const char* buffer = (const char*) toWrite ; 
    m_out_stream->write(buffer,count); 
} // write_chars





