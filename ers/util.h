/** @file This files contains small utility functions and general definitions for ers.
  */


#ifndef __ERS_UTIL__
#define __ERS_UTIL__  "0.1"

#include <string>
#include <map>

namespace ers {
    
    typedef enum _ers_severity { ers_debug_0, ers_debug_1, ers_debug2, ers_debug3, ers_information, ers_notification, ers_warning, ers_error, ers_fatal, ers_severity_none, ers_severity_max } ers_severity ; 
    typedef enum _ers_responsibility { ers_resp_client, ers_resp_server, ers_resp_dependency , ers_resp_unknown, ers_resp_max } ers_responsibility ; 
    typedef std::map<std::string, std::string> string_map_type ;   
    const char* get_severity_text(ers_severity s); 
    ers_severity parse_severity(const char *s) ; 
    ers_severity parse_severity(const std::string &s);
    const char* get_responsibility_text(ers_responsibility r) ; 
    ers_responsibility parse_responsibility(const char *s) ;
    ers_responsibility parse_responsibility(const std::string &s) ;
    int parse_boolean(const char* s) ; 
    const char* get_boolean(bool b); 
   
}

#endif
