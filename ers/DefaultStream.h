/*
 *  DefaultStream.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
 
#include <ostream>
#include <sstream>

#include "ers/Stream.h"

namespace ers {

/** This class streams an issue into an human readable string. 
  * It is uses by the issue class to display itself and to put it into standard stl streams. 
  * \author Matthias Wiesmann
  * \version 1.0
  * \brief Single line, human readable format stream. 
  */
    
class DefaultStream  : public Stream {
protected:
   
public:
    static const char * const KEY ;
    DefaultStream() ;
    ~DefaultStream() ; 
    void send(const Issue *ptr); 
} ; // DefaultStream 

} // namespace ers

