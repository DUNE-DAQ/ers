/*
 *  MapFile.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 07.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/File.h"

namespace ers {
    class MapFile : public File {
	
protected:
	void *  m_map_address ; 
	size_t  m_map_size ; 
	size_t  m_map_offset ; 
	int     m_map_fd ; 
	mode_t  m_map_permission ; 
	bool    m_map_read ; 
	bool    m_map_write ;
	void    open_fd(); 
	void    map_mem(); 
public:
	MapFile(const std::string &file, size_t size, size_t offset, bool read_mode, bool write_mode, mode_t permissions = 0666) ;
	~MapFile(); 
	void map(); 
	void unmap(); 
	void *address();
	size_t size(); 
    } ; // MapFile
    
} // ers
