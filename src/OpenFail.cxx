
#include <iostream>
#include <sstream>


#include <fcntl.h>

#include "ers/OpenFail.h"
#include "ers/Precondition.h"

const char* ers::OpenFail::FLAG_KEY = "POSIX_OPEN_FAIL_FLAG" ; 
const char* ers::OpenFail::OPENFAIL_CLASS = "ers::OpenFail" ;


namespace {
    ers::Issue *create_open_fail_issue() { return new ers::OpenFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue(ers::OpenFail::OPENFAIL_CLASS,create_open_fail_issue) ;
} 

std::string ers::OpenFail::open_fail_message(const char* path, const std::string &host, const std::string &msg) {
    std::ostringstream message ;
    message << "cannot open file '" << path << "' on " << host << ": " << msg ;
    return message.str(); 
} // open_fail_message

std::string ers::OpenFail::open_flag_message(int flags) {
    std::ostringstream flag_str ; 
    if (flags & O_WRONLY) {
        flag_str << "WRITE" ; 
    } else if (flags & O_RDWR) {
        flag_str << "READ-WRITE" ; 
    } else {
        flag_str << "READ" ; 
    }
    if (flags & O_NONBLOCK) {
        flag_str << "/NOBLOCK" ; 
    } 
    if (flags & O_APPEND) {
        flag_str << "/APPEND" ; 
    }
    if (flags & O_CREAT) {
        flag_str << "/CREATE" ; 
    }
    if (flags & O_TRUNC) {
        flag_str << "/TRUNCATE" ; 
    } 
    if (flags & O_EXCL) {
        flag_str << "/EXCLUSIVE" ; 
    }

    flag_str << "(" << flags << ")" ; 
    return flag_str.str();
} // open_flag_message

ers::OpenFail::OpenFail(const Context &c, int flags, const char *path) : FileIssue(c,ers_error,path) {
    ERS_PRECONDITION(path!=0,"Null path pointer"); 
    setup(path, flags); 
} // open_fail

ers::OpenFail::OpenFail(const Context &c, int flags, const char *path, const std::exception *cause) : FileIssue(c, ers_error, path,cause) {
    ERS_PRECONDITION(path!=0,"Null path pointer"); 
    setup(path, flags); 
} // open_fail


ers::OpenFail::OpenFail() : FileIssue() {} // open_fail

const char* ers::OpenFail::get_class_name() const throw () { return OPENFAIL_CLASS ;} 

void ers::OpenFail::setup(const char *path, int flags) {
    m_value_table[FLAG_KEY] = open_flag_message(flags); 
    responsibility(ers_resp_client); 
    std::string host = get_value(HOST_NAME_KEY); 
    std::string message = open_fail_message(path,host,posix_message());
    finish_setup(message);
}  // setup

