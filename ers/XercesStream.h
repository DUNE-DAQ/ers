/*
 *  XercesStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 12.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/STLStream.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/XMLFormatter.hpp>


XERCES_CPP_NAMESPACE_USE

namespace ers {
    
    /** 
      * This clas represents a stream that reads and writes Issues to an XML stream using the Xerces XML Framework 
      * \author Matthias Wiesmann
      * \version 1.0 
      * \brief XML stream
      */
    
    class XercesStream : 
    public STLStream , 
    public DOMErrorHandler , 
    public XMLFormatTarget {
	
protected:
	std::string m_file_path ;                                   /**< path of the file, if applicable, used for information only */                     
	static ers::Issue to_issue(const DOMError& domError); 
	static std::string get_text(const DOMNode *node)  ; 
	static std::string get_text(const DOMElement *node)  ; 
	static std::string error_msg(const DOMNode *node); 
	void cannot_parse(const DOMNode *node) const ; 
	void parse(const DOMNode *node, std::string &key, string_map_type &values, const ers::Issue **cause) const ;
	Issue *receive(const DOMElement *issue_element_ptr) const ; 
	Issue *receive(const DOMDocument *document_ptr) const ; 
	void add_string_tag(DOMElement *element_ptr, const char*name, const char* value) ; 
	void serialize(DOMElement *element_ptr, const Issue *issue_ptr); 
	void send(DOMDocument *document_ptr, const Issue *issue_ptr) ; 
	MemBufInputSource *get_source() ; 
public: 
	static const char * const XML_TAGS[] ; 
	static const char * const XML_SUFFIX ;          /**< Key for XML format */
	virtual void writeChars(const XMLByte* const toWrite, const unsigned int count,XMLFormatter* const formatter);
	bool handleError(const DOMError& domError) ;
	XercesStream(std::ostream *out_stream) ; 
	XercesStream(const System::File &file, bool read_mode) ; 
	~XercesStream() ; 
	virtual void send(const Issue *i) ;        
	virtual Issue *receive() ; 
	
    } ; // XMLOutStream
    
} // ers

