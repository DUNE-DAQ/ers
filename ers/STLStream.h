/*
 *  STLStream.h
 *  ERS
 *
 *  Created by Matthias Wiesmann on 28.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_STL_STREAM
#define ERS_STL_STREAM

#include "ers/Stream.h"
#include <iostream>
#include <fstream>

#include "system/File.h"

namespace ers {
    class STLStream : public Stream {
private:
	STLStream(const STLStream &other) {} 
protected:
	std::istream *m_in_stream ;                                    /**< \brief internal input stream */
	std::ostream *m_out_stream ;                                   /**< \brief internal output stream */
	bool m_delete_in_stream ; 
	bool m_delete_out_stream ; 
	void open_stderr();
	void open_stdin(); 
	void open_read(const System::File &file) ;
	void open_write(const System::File &file) ; 
	virtual void serialize_start(const Issue *issue_ptr) ;           /**< \brief serialise the start of an issue */
	virtual void serialize_end(const Issue *issue_ptr) ;            /**< \brief serialise the end of an issue */
	virtual void serialize_separator(const Issue *issue_ptr) ;      /**< \brief serialise a separator between a set of key / values */  
	virtual void serialize_pair(const Issue* issue_ptr, const std::string &key, const std::string &value) ;  /**< \brief serialise a key / value pair */	
	virtual bool deserialize(string_map_type *values) ;  /** \brief deserialise a set of properties from the stream */
public:
	STLStream() ;
	STLStream(std::istream *in_stream) ; 
	STLStream(std::ostream *out_stream) ; 
	STLStream(const System::File &file, bool read_mode) ; 
	STLStream(const void* ptr) ;
	~STLStream(); 
	std::string str() const ;     
	virtual void send(const Issue *issue_ptr) ; 
	virtual Issue *receive() ; 
    } ; // STLStream
} // ers

#endif

