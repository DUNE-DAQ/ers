/*
 *  STLInStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 09.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Stream.h"
#include <iostream>
#include <fstream>

namespace ers {
    
    /** This abstract class represents a Issue Stream that stream issues out of a STL stream 
    * @author Matthias Wiesmann
    */
    
    class STLInStream : public Stream {
protected:
	std::istream *_stream ;           
	bool delete_stream ;
	bool read_properties(string_map_type &values) = 0; 
public:
	void send(const Issue *i) ; 
	Issue *receive() ; 
	STLOutStream(std::istream *s);
	STLOutStream(const char* filename) ; 
	STLOutStream(); 
	~STLOutStream() ; 
    } ; // Stream_Stream
} // ers