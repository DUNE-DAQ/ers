/*
 *  TabInStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 17.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include "ers/STLInStream.h"

namespace ers {    
    
  /** Simple stream that writes data as tab separated entries
    * @author Matthias Wiesmann
    * @version 0.1
    */
    
    class TabInStream : public STLInStream {
	
protected:
	bool read_line(std::string &key, std::string &value) ; 
public: 
	virtual bool read_properties(string_map_type *values) ;
	TabInStream(const char* filename) ; 
	TabInStream(); 
    } ; // TabInStream
    
} // ers 
