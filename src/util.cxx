#include <assert.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "ers/util.h"
#include "ers/Assertion.h"

const char* ers_severity_text_names[] = { "debug_0", "debug_1", "debug_2", "debug_3", "information", "notification", "warning", "error", "fatal", "none" } ;
const char* ers_responsibility_text_names[] = { "precondition", "internal", "subsystem" } ; 
const char* boolean_names[] = { "false", "true" } ; 

/** @brief Transforms a severity type into the corresponding string
* @param s severity
* @return pointer to string with associated text 
*/

const char *ers::get_severity_text(ers::ers_severity s) {
    const unsigned int index = (unsigned int) s ;
    assert(index<=ers_severity_max); 
    return ers_severity_text_names[index] ; 
} // getSeverityText

/** Parses a string and extracts a severity 
 * @param s the string to parse 
 * @return a severity value
 */

ers::ers_severity ers::parse_severity(const char *s) {
    for(int i=0;i<ers_severity_max;i++) {
        if (strcmp(s,ers_severity_text_names[i])==0) return (ers::ers_severity) i ; 
    }
    return ers_severity_none ; 
} // parse_severity

/** Parses a string and extracts a severity 
 * @param s the string to parse 
 * @return a severity value
 */

ers::ers_severity ers::parse_severity(const std::string &s) {
    return ers::parse_severity(s.c_str());
} // parse_severity

/** Transforms a responsibility value into a string 
  * @param r the responsibility
  * @return string with text for responsibility 
  */

const char* ers::get_responsibility_text(ers::ers_responsibility r) {
    const unsigned int index = (unsigned int) r ;
    assert(index<=ers_resp_max);
    return ers_responsibility_text_names[index] ;
} // get_responsability_text

/** Parses a string and extracts a responsibility 
 * @param s the string to parse 
 * @return a responsibility value
 */

ers::ers_responsibility ers::parse_responsibility(const char *s) {
    for(int i=0;i<ers_resp_max;i++) {
        if (strcmp(s,ers_responsibility_text_names[i])==0) return (ers::ers_responsibility) i ; 
    }
    return ers_resp_unknown ;
} // parse_responsability

/** Parses a string and extracts a responsibility 
  * @param s the string to parse
  * @return a responsibility value
  */

ers::ers_responsibility ers::parse_responsibility(const std::string &s) {
    return ers::parse_responsibility(s.c_str()) ; 
} // parse_responsability

int ers::parse_boolean(const char* s) {
    if (! s) return -1 ; 
    if (! *s) return -1 ; 
    if (strcasecmp(s,"true")==0) return 1 ;
    if (strcasecmp(s,"false")==0) return 0 ; 
    return -1 ; 
} // parse_boolean

const char* ers::get_boolean(bool b) {
    int index = b ? 1 : 0 ;
    return boolean_names[index] ;
} // get_boolean


