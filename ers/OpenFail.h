/*
 *  OpenFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_OPEN_FAIL
#define ERS_OPEN_FAIL

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "ers/FileIssue.h"

namespace ers {
    
    /** This class marks that an open posix call failed.
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
class OpenFail : public FileIssue {
protected:
    static std::string open_fail_message(const char* path, const std::string &host, const std::string &message) ;
    void setup(const char*path, int flags); 
public:
    static const char* FLAG_KEY ; 
    static const char* OPENFAIL_CLASS ; 
    OpenFail(); 
    OpenFail(const Context &c, int flags, const char *path) ; 
    OpenFail(const Context &c, int flags, const char *path, const std::exception *cause) ; 
    OpenFail(const Context &c, int flags, const std::string &path, const std::exception *cause) ; 

    virtual const char *get_class_name() const throw();
} ; // open_fail   
        
} // namespace ers 

#define ERS_OPEN_READ_FAIL(path,...)  ers::OpenFail(ERS_HERE, O_RDONLY, path, __VA_ARGS__)  
#define ERS_OPEN_WRITE_FAIL(path,...) ers::OpenFail(ERS_HERE, O_WRONLY, path, __VA_ARGS__) 

#endif
