/*
 *  TabStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 28.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/STLStream.h"

namespace ers {
    
    /** This stream is responsible for writing and reading data from tab separated format
      * \author Matthias Wiesmann
      * \brief tab format issue stream 
      * \version 1.1 (merged in and out classes)
      */
    
    class TabStream : public STLStream {
protected:
	bool read_line(std::string &key, std::string &value) ;  /**< \brief reads one line and parses key and value */
	virtual bool deserialize(string_map_type *values) ;     /**< \brief loads key and values into a value_table */
	virtual void serialize_end(const Issue *i) ;
	virtual void serialize_pair(const Issue *i, const std::string &key, const std::string &value) ; 
public:
	static const char* TAB_SUFFIX ;                         /**< Key for Tabulated format */
        TabStream();  
	TabStream(std::istream *in_stream) ; 
	TabStream(std::ostream *out_stream) ; 
	TabStream(const System::File &file, bool read_mode); 
	~TabStream();
    } ; // TabStream
} ; // ers

