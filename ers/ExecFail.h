/*
 *  ExecFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/FileIssue.h"

namespace ers {
    class ExecFail : public FileIssue {
protected:
	
public:
	static const char* CLASS_NAME ; 
	ExecFail(); 
	ExecFail(const Context &c, const char*path) ; 
	virtual const char *get_class_name() const throw();
    } ; // UnlinkFail
} // ers ; 

#define ERS_EXEC_ERROR(path) ers::ExecFail(ERS_HERE,path)
