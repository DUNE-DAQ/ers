/*
 *  UnlinkFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/FileIssue.h"

namespace ers {
    class UnlinkFail : public FileIssue {
public:
	static const char* CLASS_NAME ; 
	UnlinkFail(); 
	UnlinkFail(const Context &c, const char*path) ; 
	virtual const char *get_class_name() const throw();
    } ; // UnlinkFail
} // ers ; 

#define ERS_UNLINK_ERROR(path) ers::UnlinkFail(ERS_HERE,path)


