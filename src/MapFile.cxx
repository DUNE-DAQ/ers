/*
 *  MapFile.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 07.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <sys/types.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>

#include "ers/MapFile.h"
#include "ers/Precondition.h"
#include "ers/OpenFail.h"
#include "ers/NotImplemented.h"
#include "ers/MMapFail.h"


ers::MapFile::MapFile(const std::string &name, size_t size, size_t offset, bool read_mode, bool write_mode, mode_t permissions) : File(name) {
    ERS_PRECONDITION((read_mode || write_mode),"neither read nor write mode"); 
    const int page_size = getpagesize();
    ERS_PRECONDITION((size%page_size)==0,"Size %d is not a multiple of pagesize %d",(int) size, page_size);
    ERS_PRECONDITION((offset%page_size)==0,"Offset %d is not a multiple of pagesize %d",(int) offset,page_size);
    m_map_read = read_mode ; 
    m_map_write = write_mode ; 
    m_map_permission = permissions ; 
    m_map_size = size ; 
    m_map_offset = offset ;
    m_map_address = 0 ; 
    m_map_fd = -1 ; 
} // MapFile

ers::MapFile::~MapFile() {
    if (m_map_fd) {
	std::cerr << "Warning fd not closed" << std::endl ; 
    }
} // 

void ers::MapFile::open_fd() {
    int flags = 0 ; 
    if (m_map_read && m_map_write) { 
	flags = O_RDWR ; 
    } else if (m_map_read) {
	flags = O_RDONLY ; 
    } else if (m_map_write) {
	flags = O_WRONLY ; 
    } else {
	NOT_IMPLEMENTED();  // map neither in read or write should never occur.
    } // 
    const char* name = m_full_name.c_str(); 
    m_map_fd = ::open(name,flags,m_map_permission); 
    if (m_map_fd<0) throw ers::OpenFail(ERS_HERE,flags,name) ; 
} // open_fd 

void ers::MapFile::map_mem() {
    ERS_PRECONDITION(m_map_fd>=0,"invalid file descriptor %d",m_map_fd); 
    int prot = 0 ; 
    int flags = MAP_FILE ; 
    if (m_map_read)  { 
	prot|=PROT_READ ;  
    } 
    if (m_map_write) { 
	prot|=PROT_WRITE ;
	flags|=MAP_SHARED ; 
    }
    m_map_address = ::mmap(0,m_map_size,prot,flags,m_map_fd,m_map_offset); 
    if (m_map_address==MAP_FAILED || m_map_address==0) {
	throw ERS_MMAP_ERROR(m_map_size,prot,flags,m_map_fd,m_map_offset); 
    } // map_failed 
} // map_mem

void ers::MapFile::map() {
    open_fd();
    map_mem(); 
} // map 

void ers::MapFile::unmap() {} 
void *ers::MapFile::address() { return m_map_address ; }
size_t ers::MapFile::size() { return m_map_size ; }


