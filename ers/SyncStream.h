/*
 *  SyncStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Stream.h"

namespace ers {

    /** This streams is used to transmit Issues inside a process.
      * @author Matthias Wiesmann
      * @version 0.1 (blocked).
      * @note This class is only partially implemented, as it requires a thread library of some sort. 
      */
    
    class SyncStream : public Stream {
public:
	void send(const Issue *i) ;
	Issue *receive() ; 
	SyncStream();
	~SyncStream() ;
protected:
    } ; //  Sync_Stream
} // ers
	

