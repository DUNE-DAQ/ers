/*
 *  GetWDFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_GET_WD_FAIL
#define ERS_GET_WD_FAIL

#include "ers/FileIssue.h"

namespace ers {
    
  /** This class marks that an \c getwd call failed.
    * \author Matthias Wiesmann
    * \version 1.0
    */
    
    class GetWDFail : public IOIssue {
protected:
public:
	static const char* CLASS_NAME ; 
	GetWDFail(); 
	GetWDFail(const Context &c) ; 
	virtual const char *get_class_name() const throw();
    } ; // GetWDFail   
    
} // namespace ers 

#define ERS_GET_WD_ERROR() ers::GetWDFail(ERS_HERE)

#endif
