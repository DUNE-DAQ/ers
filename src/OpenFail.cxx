#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ers/OpenFail.h"
#include "ers/Precondition.h"

namespace {
    ers::Issue *create_open_fail_issue() { return new ers::OpenFail();  } 
    bool registered = ers::IssueFactory::instance()->register_issue("ers::open_fail",create_open_fail_issue) ;
} 

using namespace ers ; 

const char* OpenFail::PATH_KEY = "OPEN_FAIL_PATH" ; 
const char* OpenFail::FLAG_KEY = "OPEN_FAIL_FLAG" ; 
const char* OpenFail::OPENFAIL_CLASS = "ers::OpenFail" ;

std::string OpenFail::open_fail_message(const char* path, const std::string &host, const std::string &msg) {
    std::ostringstream message ;
    message << "cannot open file '" << path << "' on " << host << ": " << msg ;
    return message.str(); 
} // open_fail_message

std::string OpenFail::open_flag_message(int flags) {
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

OpenFail::OpenFail(const char *path, int flags, const Context &context) : 
PosixIssue(ers_error,context) {
    ERS_PRECONDITION(path!=0,"Null path pointer"); 
    m_value_table[PATH_KEY] = std::string(path);
    m_value_table[FLAG_KEY] = open_flag_message(flags); 
    responsibility(ers_resp_client); 
    std::string host = get_value(HOST_NAME_KEY); 
    std::string message = open_fail_message(path,host,posix_message());
    finish_setup(message);
} // open_fail

OpenFail::OpenFail() : PosixIssue() {} // open_fail

const char* OpenFail::get_class_name() const throw () { return OPENFAIL_CLASS ;} 


