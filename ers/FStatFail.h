/*
 *  FStatFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_FSTAT_FAIL
#define ERS_FSTAT_FAIL

#include "ers/FileIssue.h"

namespace ers {
    
    /** This class marks that an \c getwd call failed.
    * \author Matthias Wiesmann
    * \version 1.0
    */
    
    class FStatFail : public FileIssue {
protected:
public:
	static const char* CLASS_NAME ; 
	FStatFail(); 
	FStatFail(const Context &c, const char*path) ; 
	virtual const char *get_class_name() const throw();
    } ; // GetWDFail   
    
} // namespace ers 

#define ERS_FSTAT_ERROR(path) ers::FStatFail(ERS_HERE,path)

#endif

