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
    
    /** This class contains some general constants.
      * \brief Miscalenous constants
      */
    class Core {
	static const char* const SEVERITY_NAMES[] ;
	static const char* const BOOLEAN_NAMES[] ; 
	static const char* const RESPONSIBILITY_NAMES[] ; 
protected:
public:
	static const std::string empty_string ;
	static ers_severity parse_severity(const char *s) throw() ;                     /**< \brief string to severity */
	static ers_severity parse_severity(const std::string &s) throw() ;              /**< \brief string to severity */
	static const char* to_string(ers_severity s) throw() ;                          /**< \brief severity to string */
	static ers_responsibility parse_responsibility(const char *s) throw() ;         /**< \brief string to responsibility */
	static ers_responsibility parse_responsibility(const std::string &s) throw() ;  /**< \brief string to responsibility */
	static const char* to_string(ers_responsibility s) throw() ;                    /**< \brief responsibility to string */
	static int parse_boolean(const char* s) throw() ;                               /**< \brief string to boolean */
	static const char* to_string(bool b) throw ();                                  /**< \brief boolean to string */
	static std::string parse_prefix_string(const char **ptr) throw() ;                    /**< \brief prefix string data to string */
	static std::string umangle_gcc_class_name(const char* name) throw() ;           /**< \brief unmangles gcc RTTI names */
    } ; // Core
}

#endif
