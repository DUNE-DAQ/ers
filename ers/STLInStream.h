/*
 *  STLInStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 09.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Stream.h"
#include <iostream>
#include <fstream>

namespace ers {
    
    /** This abstract class represents a Issue Stream that stream issues out of a STL stream 
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
    class STLInStream : public Stream {
protected:
	std::istream *m_stream ;                                    /**< \brief internal output stream */
	bool m_delete_stream ;                                      /**< \brief should the stream be destroyed when this object is destructed */
	virtual bool read_properties(string_map_type *values) = 0;  /**< \brief Abstract method, should reads properties from the stream */
public:
	void send(const Issue *i) ;                                 /**< \brief Sends an issue to the stream */
	Issue *receive() ;                                          /**< \brief Reads an issue from the stream */
	STLInStream(std::istream *s);                               /**< \brief Builds a ERS stream with a STL input stream */
	STLInStream(const char* filename) ;                         /**< \brief Builds an ERS stream that reads from a file */
	STLInStream();                                              /**< \breif Builds an ERS stream that reads from \c stdin */
	~STLInStream() ; 
    } ; 
} // ers


