/*
 *  TabStream.h
 *  ERS
 *
 *  Created by Matthias Wiesmann on 28.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/STLStream.h"

namespace ers {
    class TabStream : public STLStream {
protected:
	bool read_line(std::string &key, std::string &value) ;  /**< \brief reads one line and parses key and value */
	virtual bool deserialize(string_map_type *values) ;     /**< \brief loads key and values into a value_table */
	virtual void serialize_end(const Issue *i) ;
	virtual void serialize_pair(const Issue *i, const std::string &key, const std::string &value) ; 
public:
        TabStream();  
	TabStream(std::istream *in_stream) ; 
	TabStream(std::ostream *out_stream) ; 
	TabStream(const void *ptr) ;
	TabStream(const System::File &file, bool read_mode); 
	~TabStream();
    } ; // TabStream
} ; // ers

