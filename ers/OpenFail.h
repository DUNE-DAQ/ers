/*
 *  OpenFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include "ers/FileIssue.h"

namespace ers {
    
class OpenFail : public FileIssue {
protected:
    static std::string open_fail_message(const char* path, const std::string &host, const std::string &message) ;
    static std::string open_flag_message(int flags) ; 
public:
    static const char* FLAG_KEY ; 
    static const char* OPENFAIL_CLASS ; 
    OpenFail(); 
    OpenFail(const char *path, int flags, const Context &context) ; 
    virtual const char *get_class_name() const throw();
} ; // open_fail   
        
} // namespace ers 

