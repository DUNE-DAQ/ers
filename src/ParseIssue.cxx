/*
 *  ParseIssue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 17.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/ParseIssue.h"

const char *ers::ParseIssue::OFFENDING_LINE_KEY = "PARSE_ISSUE_LINE" ; 
const char *ers::ParseIssue::PARSE_ISSUE_CLASS_NAME = "ers::ParseIssue" ; 

namespace {
    ers::Issue *create_parse_issue() { return new ers::ParseIssue(); } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::ParseIssue::PARSE_ISSUE_CLASS_NAME,create_parse_issue) ;
} 

ers::ParseIssue::ParseIssue() : ers::Issue() {} 

ers::ParseIssue::ParseIssue(const Context &c, ers_severity s) : ers::Issue(c,s) {} 

ers::ParseIssue::ParseIssue(const Context &c, ers_severity s,const std::string &message ) : ers::Issue(c,s) {
    finish_setup(message); 
} // ParseIssue

ers::ParseIssue::ParseIssue(const Context &c, ers_severity s,const std::string &message, const std::string &line )  : ers::Issue(c,s) {
    this->offending_line(line); 
} // ParseIssue
 
const char* ers::ParseIssue::get_class_name() const throw() {
    return PARSE_ISSUE_CLASS_NAME ; 
} // get_class_name

void ers::ParseIssue::offending_line(const std::string &line) {
    this->m_value_table[OFFENDING_LINE_KEY] = line ; 
}// offending_line

