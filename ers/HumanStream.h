/*
 *  HumanStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include <ostream>
#include <sstream>

#include "STLStream.h"

namespace ers {

/** This class streams an issue into an human readable string. 
  * It is uses by the issue class to display itself and to put it into standard stl streams. 
  * \author Matthias Wiesmann
  * \version 1.0
  */
    
class HumanStream  : public STLStream {
protected:
    virtual void serialize_pair(const Issue *issue, const std::string &key, const std::string &value) ;
    virtual void serialize_start(const Issue *issue) ; 
    virtual void serialize_end(const Issue *issue) ; 
    virtual void serialize_separator(const Issue *issue) ;
public:
    HumanStream() ;
    HumanStream(std::ostream *s);
    HumanStream(const System::File &file, bool read_mode);
} ; // human_stream 

} // namespace ers

