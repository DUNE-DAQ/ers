/*
 *  FIFOStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <deque>
#include "ers/Stream.h"

namespace ers {

    /** This streams is used to transmit Issues inside a process.
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief Queue stream 
      * \note This stream is \b not thread safe. 
      */
    
    class FIFOStream : public Stream {
	
protected:
	std::deque<Issue*> m_issue_queue ; 
public:
	static const char* FIFO_STREAM_KEY ;     /**< Key for FIFO stream */
	void send(const Issue *i) ;
	Issue *receive() ; 
	FIFOStream();
	~FIFOStream() ;
    } ; //  FIFOStream
} // ers
	

