/*
 *  AllocIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 07.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/PosixIssue.h"

namespace ers {
    
    /** This class represents problems with memory allocation,
      * typically, failures of the functions of the \c malloc familly. 
      * Such issues are by default considered transient. 
      * \author Matthias Wiesmann
      * \version 1.0 
      */
    
    class AllocIssue : public PosixIssue {
protected:
	void requested_memory(size_t size); 
	void setup_message(size_t size); 
public:
	static const char* CLASS_NAME ; 
	static const char* POSIX_MALLOC_SIZE_KEY ; 
	AllocIssue(); 
	AllocIssue(const Context &c, size_t size=0) ; 
	AllocIssue(const Context &c, size_t number, size_t size) ; 
	virtual const char *get_class_name() const throw();
    } ; // AllocIssue
} // ers ; 

#define ERS_ALLOC_ERROR(...) ers::AllocIssue(ERS_HERE,__VA_ARGS__) 

#define ERS_ALLOC_CHECK(p,...) {if(0==p) throw ers::AllocIssue(ERS_HERE,__VA_ARGS__);}
