#include <iostream>
#include <fstream>

#include "ers/STLOutStream.h"

namespace ers {

   /** Simple issue stream that writes the issues as xml entries into a file 
    * @author Matthias Wiesmann
    * @version 0.1 (test).
    */
    
class XMLOutStream : public STLOutStream {
public:
    XMLOutStream(const char* filename) ; 
    XMLOutStream(); 
    void serialize_start(const Issue *i) ; 
    void serialize_end(const Issue *i) ;
    void serialize(const std::string &key, const std::string &value) ; 
} ; // file_stream
    
} // ers

