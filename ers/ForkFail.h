/*
 *  ForkFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/PosixIssue.h"

namespace ers {
    class ForkFail : public PosixIssue {
protected:
	
public:
	static const char* CLASS_NAME ; 
	ForkFail(); 
	ForkFail(const Context &c) ; 
	virtual const char *get_class_name() const throw();
    } ; // ForkFail
} // ers ; 

#define ERS_FORK_ERROR() ers::ForkFail(ERS_HERE) 