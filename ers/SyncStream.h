/*
 *  sync_stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Stream.h"

namespace ers {

    class SyncStream : public Stream {
public:
	void send(const Issue *i) ;
	Issue *receive() ; 
	SyncStream();
	~SyncStream() ;
protected:
    } ; //  Sync_Stream
} // ers
	

