/*
 *  XercesString.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

namespace ers {


/** This is a simple class that lets us do easy (though not terribly efficient)
  * trancoding of char* data to XMLCh data.
  * \author Apache Software Foundation
  * \note This is adapted from the \c CreateDOMDocument example for Xerces 
  */  

class XercesString {
public :
    XercesString(const char* const toTranscode) { fUnicodeForm = XMLString::transcode(toTranscode); } 
    ~XercesString() { XMLString::release(&fUnicodeForm); }
    const XMLCh* unicodeForm() const { return fUnicodeForm; }
private :
    XMLCh*   fUnicodeForm;
};


/** This function converts Xerces unicode data to a C++ string
  * \param data the unicode data 
  * \return a STL string
  */

std::string to_string(const XMLCh* data) {
    if (data) {
	char *c_tag = XMLString::transcode(data); 
	std::string str(c_tag);
	XMLString::release(&c_tag);
	return str ; 
    } else {
	return "" ; 
    } // if data
} // to_string

} // ers

/** \def to_unicode(str) converts a char* to unicode form */

#define to_unicode(str) XercesString(str).unicodeForm()




