/*
 *  RenameFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/RenameFail.h"

#include <sstream>

const char * ers::RenameFail::CLASS_NAME = "ers::RenameFail" ;
const char * ers::RenameFail::DESTINATON_KEY = "POSIX_FILE_DEST_PATH" ; 

namespace {
    ers::Issue *create_issue() { return new ers::RenameFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::RenameFail::CLASS_NAME,create_issue) ;
} 

ers::RenameFail::RenameFail() : FileIssue() {} 
ers::RenameFail::RenameFail(const Context &c,const char *src, const char* dst) : FileIssue(c,ers_error,src) {
    destination(dst); 
    std::ostringstream message ;
    message << "cannot rename to '" << dst << "'" ; 
    setup_file_issue(message.str(),src);
} // UnlinkFail

void ers::RenameFail::destination(const char* dst) {
    m_value_table[DESTINATON_KEY] = dst ; 
} // destination

const char *ers::RenameFail::get_class_name() const throw() {
    return ers::RenameFail::CLASS_NAME ; 
} // UnlinkFail