/*
 *  PosixIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include "ers/PosixIssue.h"

namespace {
    ers::Issue *create_issue() { return new ers::PosixIssue(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::PosixIssue::POSIX_ERROR_CLASS,create_issue) ;
} 

#define BUFFER_SIZE 256

using namespace ers ; 

const char* PosixIssue::UNKNOWN_ERR_MSG = "Unknown error: %d" ; 
const char* PosixIssue::TO_LONG_ERR_MSG = "Error message to long for %d" ; 
const char* PosixIssue::UNKNOWN_ERR = "Unknow error trying to find error message for %d: %d" ; 
const char* PosixIssue::POSIX_ERROR_MSG_KEY = "POSIX_ERROR_MSG" ; 
const char* PosixIssue::POSIX_ERROR_NBR_KEY = "POSIX_ERROR_NBR" ;
const char* PosixIssue::POSIX_ERROR_CLASS = "ers:PosixIssue" ; 

/** Builds a string message from an error number
 *  @param err error number
 *  @return string object containing the message
 *  @note wrapper arround strerror_r with some error checking 
 */

std::string PosixIssue::error_message(int err) {
    // We cannot use the thread safe method because of conflicting definitions for strerror_r
    return std::string(strerror(err)); 
} // error_message

/** Setup method, build a string error message and inserts it and the original error number into the optional values hashmap
  * @param err error number
  * @param message the user message
  * @see error_message()
  */

void PosixIssue::setup_posix(int err) {
    std::string msg = error_message(err);
    m_value_table[POSIX_ERROR_MSG_KEY] = msg ;
    std::ostringstream error_num ;
    error_num << err ;
    m_value_table[POSIX_ERROR_NBR_KEY] = error_num.str() ; 
} // setup_errno

PosixIssue::PosixIssue() : Issue() {} 

PosixIssue::PosixIssue(const Context &c, ers_severity s, const std::string &message, int err) : Issue(c,s) {
    setup_posix(err);
    finish_setup(message); 
} // posix_issue

PosixIssue::PosixIssue(const Context &c, ers_severity s, const std::string &message) : Issue(c,s) {
    setup_posix(errno);
    finish_setup(message); 
} // posix_issue

PosixIssue::PosixIssue(const Context &c ,ers_severity s) : Issue(c,s) {
    setup_posix(errno);
} // posix_issue

PosixIssue::PosixIssue(const Context &c, ers_severity s, const std::string &message, const std::exception *cause) : Issue(c,s,cause) {
    setup_posix(errno);
    finish_setup(message);
} // PosixIssue

PosixIssue::PosixIssue(const Context &c, ers_severity s, const std::exception *cause ) : Issue(c,s,cause) {
    setup_posix(errno);
}  // PosixIssue


const char*PosixIssue::get_class_name() const throw() {return POSIX_ERROR_CLASS ; } 

std::string PosixIssue::posix_message() const throw() {
    return get_value(POSIX_ERROR_MSG_KEY); 
} // posix_message



