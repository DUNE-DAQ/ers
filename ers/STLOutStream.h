/*
 *  stl_out_stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Stream.h"
#include <iostream>
#include <fstream>


namespace ers {
    
    /** This abstract class represents a Issue Stream that stream issues into a STL stream 
      * @author Matthias Wiesmann
      */
    
    class STLOutStream : public Stream {
protected:
	std::ostream *_stream ;           
	bool delete_stream ;
	void serialize_separator(const Issue *i) ;          
	void serialize_start(const Issue *i) ;
	void serialize_end(const Issue *i) ;
	virtual void serialize(const std::string &key, const std::string &value) = 0 ; 
public:
	void send(const Issue *i) ; 
	Issue *receive() ; 
	STLOutStream(std::ostream *s);
	STLOutStream(const char* filename) ; 
	STLOutStream(); 
	~STLOutStream() ; 
    } ; // Stream_Stream
} // ers
