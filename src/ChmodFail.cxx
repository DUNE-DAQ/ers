/*
 *  ChmodFail.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/ChmodFail.h"
#include "ers/File.h"

#include <sstream>


const char * ers::ChmodFail::CLASS_NAME = "ers::ChmodFail" ;
const char * ers::ChmodFail::PERMISSION_KEY = "POSIX_PERMISSION" ; 

namespace {
    ers::Issue *create_issue() { return new ers::ChmodFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::ChmodFail::CLASS_NAME,create_issue) ;
} 

ers::ChmodFail::ChmodFail() : FileIssue() {} 

ers::ChmodFail::ChmodFail(const Context &c,const char *p,mode_t perm) : FileIssue(c,ers_error,p) {
    permissions(perm); 
    std::ostringstream message ;
    message << "Cannot set permissions to '" << ers::File::pretty_permissions(perm) << "' "  ; 
    setup_file_issue(message.str(),p);
} // UnlinkFail

void ers::ChmodFail::permissions(mode_t p) {
    std::ostringstream stream ;
    stream << p ; 
    m_value_table[PERMISSION_KEY] = stream.str() ; 
} // permissions

const char *ers::ChmodFail::get_class_name() const throw() {
    return ers::ChmodFail::CLASS_NAME ; 
} // UnlinkFail


