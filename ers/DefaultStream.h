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

/** This class displas an Issue on the standard error stream
  * This class is meant to be used as default stream for simple on screen display.
  * More advanced streams are offered in the \c System package 
  * \author Matthias Wiesmann
  * \version 1.0
  * \brief Default Issue display 
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

