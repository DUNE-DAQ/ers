#include <iostream>
#include <fstream>

#include "ers/Stream.h"

class TiXmlElement ; 
class TiXmlNode ; 

namespace ers {

   /** 
    * \author Matthias Wiesmann
    * \version 1.0 
    */
    
class XMLStream : public Stream {
    
protected:
    std::string m_file_name ; 
    std::string get_text(const TiXmlElement *element) const ; 
    void parse(const TiXmlElement *element, std::string &key, string_map_type &values) const ; 
    void parse(const TiXmlNode *node, std::string &key, string_map_type &values) const ; 
    void cannot_parse(const TiXmlNode *node) const ; 
public:
    static const char *ISSUE_TAG ; 
    static const char *KEY_TAG ; 
    static const char *STRING_VALUE_TAG ; 
    XMLStream(const char* filename) ; 
    ~XMLStream() ; 
    virtual void send(const Issue *i) ;        
    virtual Issue *receive() ; 
} ; // XMLOutStream
    
} // ers

