/*
 *  MMapFail.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 07.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/IOIssue.h"

namespace ers {
    
    /** This class represents a failure when doing a mmap
     * \author Matthias Wiesmann
     * \version 1.0 
     */
    
    class MMapFail : public IOIssue {
protected:
	void mem_size(size_t s) ; 
	void mem_offset(size_t o) ; 
public:
	static const char* CLASS_NAME ; 
	static const char* POSIX_MMAP_SIZE ;
	static const char* POSIX_MMAP_OFFSET ; 
	static const char* POSIX_MMAP_FLAG ; 
	MMapFail(); 
	MMapFail(const Context &c, size_t size, int prot, int flag, int fd, size_t offset) ; 
	virtual const char *get_class_name() const throw();
    } ; // AllocIssue
} // ers ; 

#define ERS_MMAP_ERROR(size,prot,flag,fd,offset) ers::MMapFail(ERS_HERE,size,prot,flag,fd,offset) 


