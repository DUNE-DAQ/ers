
#include <iostream>
#include <sstream>
#include <string.h>
#include "ers/Core.h"

const char* const ers::Core::SEVERITY_NAMES[] = { "none", "debug_0", "debug_1", "debug_2", "debug_3", "information", "notification", "warning", "error", "fatal", "maximum (illegal)" } ;
const char* const ers::Core::BOOLEAN_NAMES[] = { "false", "true" } ; 
const char* const ers::Core::RESPONSIBILITY_NAMES[] = { "precondition", "internal", "subSystem" } ; 

/** 
 * \brief Transforms a severity type into the corresponding string
 * \param s severity
 * \return pointer to string with associated text 
 */

const char *ers::Core::to_string(ers::ers_severity s) throw() {
    const unsigned int index = (unsigned int) s ;
    assert(index<=ers_severity_max); 
    return ers::Core::SEVERITY_NAMES[index] ; 
} // getSeverityText

/** Parses a string and extracts a severity 
 * \param s the string to parse 
 * \return a severity value
 */

ers::ers_severity ers::Core::parse_severity(const char *s) throw() {
    for(int i=0;i<ers_severity_max;i++) {
        if (strcmp(s,ers::Core::SEVERITY_NAMES[i])==0) return (ers::ers_severity) i ; 
    }// for
    return ers_severity_none ; 
} // parse_severity

/** Parses a string and extracts a severity 
 * \param s the string to parse 
 * \return a severity value
 */

ers::ers_severity ers::Core::parse_severity(const std::string &s) throw() {
    return parse_severity(s.c_str());
} // parse_severity

/** Transforms a responsibility value into a string 
  * \param r the responsibility
  * \return string with text for responsibility 
  */

const char* ers::Core::to_string(ers::ers_responsibility r) throw() {
    const unsigned int index = (unsigned int) r ;
    assert(index<=ers_resp_max);
    return RESPONSIBILITY_NAMES[index] ;
} // to_string

/** Parses a string and extracts a responsibility 
 * \param s the string to parse 
 * \return a responsibility value
 */

ers::ers_responsibility ers::Core::parse_responsibility(const char *s) throw() {
    for(int i=0;i<ers_resp_max;i++) {
        if (strcmp(s,RESPONSIBILITY_NAMES[i])==0) return (ers::ers_responsibility) i ; 
    } // for 
    return ers_resp_unknown ;
} // parse_responsability

/** Parses a string and extracts a responsibility 
  * \param s the string to parse
  * \return a responsibility value
  */

ers::ers_responsibility ers::Core::parse_responsibility(const std::string &s) throw() {
    return parse_responsibility(s.c_str()) ; 
} // parse_responsability

/** Parse a string and extract a boolean 
  * \param s the string to parse
  * \return 1 if true
  * \return 0 if false
  * \return -1 if undefined 
  */

int ers::Core::parse_boolean(const char* s) throw() {
    if (! s) return -1 ; 
    if (! *s) return -1 ; 
    if (strcasecmp(s,BOOLEAN_NAMES[1])==0) return 1 ;
    if (strcasecmp(s,BOOLEAN_NAMES[0])==0) return 0 ; 
    return -1 ; 
} // parse_boolean

/** Convert a boolean to a string 
  * \param b the boolean
  * \return either the string "true" or "false"
  */

const char* ers::Core::to_string(bool b) throw () {
    int index = b ? 1 : 0 ;
    return BOOLEAN_NAMES[index] ;
} // to_string







