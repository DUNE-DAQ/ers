/*
 *  ChmodFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/FileIssue.h"

namespace ers {
    class ChmodFail : public FileIssue {
protected:
	void permissions(mode_t p) ; 
public:
	static const char* CLASS_NAME ; 
	static const char* PERMISSION_KEY ; 
	ChmodFail(); 
	ChmodFail(const Context &c, const char*path, mode_t permissions) ; 
	virtual const char *get_class_name() const throw();
    } ; // UnlinkFail
} // ers ; 

#define ERS_CHMOD_ERROR(path,permissions) ers::ChmodFail(ERS_HERE,path,permissions)



