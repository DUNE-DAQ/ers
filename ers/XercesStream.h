/*
 *  XercesStream.h
 *  Test
 *
 *  Created by Matthias Wiesmann on 12.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/Stream.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>


XERCES_CPP_NAMESPACE_USE

namespace ers {
    
    /** 
    * \author Matthias Wiesmann
    * \version 1.0 
    */
    
    class XercesStream : public Stream , public DOMErrorHandler {
	
protected:
	std::string m_file_path ; 
	static ers::Issue to_issue(const DOMError& domError); 
	static std::string get_text(const DOMNode *node)  ; 
	static std::string get_text(const DOMElement *node)  ; 
	static std::string error_msg(const DOMNode *node); 

	void cannot_parse(const DOMNode *node) const ; 
	void parse(const DOMElement *element, std::string &key, string_map_type &values) const ; 
	void parse(const DOMNode *node, std::string &key, string_map_type &values) const ;
	Issue *receive(const DOMElement *issue_element_ptr) const ; 
	Issue *receive(const DOMDocument *document_ptr) const ; 
	void send(DOMDocument *document_ptr, const Issue *issue_ptr) ; 
public:
	bool handleError(const DOMError& domError) ;
	XercesStream(const System::File &file) ; 
	~XercesStream() ; 
	virtual void send(const Issue *i) ;        
	virtual Issue *receive() ; 
	
    } ; // XMLOutStream
    
} // ers

