/*
 *  stl_out_stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_STL_OUT_STREAM
#define ERS_STL_OUT_STREAM

#include "ers/Stream.h"
#include "ers/File.h"
#include <iostream>
#include <fstream>

namespace ers {
    
    /** This abstract class represents a Issue Stream that stream issues into a STL stream 
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
    class STLOutStream : public Stream {
protected:
	std::ostream *m_stream ;                       /**< \brief internal output stream */
	bool m_delete_stream ;                         /**< \brief should the stream be destroyed when this object is destructed */
	void serialize_separator(const Issue *i) ;     /**< \brief serialise a separator between a set of key / values */  
	virtual void serialize_start(const Issue *i) ; /**< \brief serialise the start of an issue */
	virtual void serialize_end(const Issue *i) ;   /**< \brief serialise the end of an issue */
	virtual void serialize(const std::string &key, const std::string &value) = 0 ; /**< \brief serialise a key / value pair */
	void open(const char* filename); 
public:
	void send(const Issue *i) ; 
	Issue *receive() ; 
	STLOutStream(std::ostream *s);
	STLOutStream(const char* filename) ; 
	STLOutStream(const std::string &filename); 
	STLOutStream(const ers::File &file); 
	STLOutStream(); 
	~STLOutStream() ; 
    } ; // Stream_Stream
} // ers

#endif
