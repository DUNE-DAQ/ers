#include <iostream>
#include <fstream>

#include "ers/STLOutStream.h"

namespace ers {

   /** Simple issue stream that writes the issues as xml entries into a file 
    * \author Matthias Wiesmann
    * \version 1.0 
    */
    
class XMLOutStream : public STLOutStream {
    
protected:
    void tag(const std::string &tag, const std::string &content); 
    void tag_open(const std::string &tag) ; 
    void tag_close(const std::string &tag) ; 
public:
    static const char *ISSUE_TAG ; 
    static const char *KEY_TAG ; 
    static const char *STRING_VALUE_TAG ; 
    XMLOutStream(const char* filename) ; 
    XMLOutStream(); 
    void serialize_start(const Issue *i) ; 
    void serialize_end(const Issue *i) ;
    void serialize(const std::string &key, const std::string &value) ; 
} ; // XMLOutStream
    
} // ers

