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
      * \version 0.1 (blocked).
      * \note This class is only partially implemented, as it requires a thread library of some sort. 
      */
    
    class FIFOStream : public Stream {
	
protected:
	std::deque<Issue*> m_issue_queue ; 
public:
	void send(const Issue *i) ;
	Issue *receive() ; 
	FIFOStream();
	~FIFOStream() ;
    } ; //  FIFOStream
} // ers
	

