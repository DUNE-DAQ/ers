/*
 *  RenameFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/FileIssue.h"

namespace ers {
    
    /** This issue signal that a move / rename operation failed 
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
    class RenameFail : public FileIssue {
protected:
	void destination(const char* dst); 
public:
	static const char* DESTINATON_KEY ; 
	static const char* CLASS_NAME ; 
	RenameFail(); 
	RenameFail(const Context &c, const char*source, const char* destination) ; 
	virtual const char *get_class_name() const throw();
    } ; // UnlinkFail
} // ers ; 

#define ERS_RENAME_ERROR(src,dest) ers::RenameFail(ERS_HERE,src,dest)
