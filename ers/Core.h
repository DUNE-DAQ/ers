/*
 *  Core.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

/** \file Core.h This files contains small utility functions and general definitions for ers.
  * \version 1.0 
  */


#ifndef __ERS_UTIL__
#define __ERS_UTIL__  "0.3"

#include <string>
#include <map>
#include <vector>

namespace ers {
    
    typedef enum _ers_severity { ers_severity_none, ers_debug_0, ers_debug_1, ers_debug_2, ers_debug_3, ers_information, ers_notification, ers_warning, ers_error, ers_fatal,  ers_severity_max } ers_severity ; 
    typedef enum _ers_responsibility { ers_resp_client, ers_resp_server, ers_resp_dependency , ers_resp_unknown, ers_resp_max } ers_responsibility ; 
    typedef std::map<std::string, std::string> string_map_type ;   
    typedef std::vector<std::string> string_vector ;
    const char* get_severity_text(ers_severity s); 
    ers_severity parse_severity(const char *s) ; 
    ers_severity parse_severity(const std::string &s);
    const char* get_responsibility_text(ers_responsibility r) ; 
    ers_responsibility parse_responsibility(const char *s) ;
    ers_responsibility parse_responsibility(const std::string &s) ;
    int parse_boolean(const char* s) ; 
    const char* get_boolean(bool b); 
    
    /** This class contains some general constants.
      * \brief Miscalenous constants
      */
    class Core {
    	public:
    	static const char* XML_ISSUE_TAG  ; 
    	static const char* XML_KEY_TAG  ; 
    	static const char* XML_STRING_VALUE_TAG  ; 
    } ; // Core
}

#endif
