/*
 *  STLOutStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_TAB_OUT
#define ERS_TAB_OUT

#include "ers/STLOutStream.h"

namespace ers {

    /** Simple stream that writes data as tab separated entries
      * @author Matthias Wiesmann
      * @version 0.1
      */
    
class TabOutStream : public STLOutStream {
    
public: 
    TabOutStream(const char* filename) ; 
    TabOutStream(); 
    void serialize_start(const Issue *i) ; 
    virtual void serialize_end(const Issue *i) ;
    void serialize(const std::string &key, const std::string &value) ; 
} ; // Tab_Stream

} // ers 

#endif

