/*
 *  LockStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 12.04.06.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
 
/** \file LockStream.h This file defines LockStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_GLOBAL_LOCK_STREAM_H
#define ERS_GLOBAL_LOCK_STREAM_H

#include <ers/OutputStream.h>
#include <mutex>

namespace ers
{

    /** This class can be used to lock several ERS output streams to prevent 
      * simultaneous output from different threads.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Lock for ERS streams.
      */

    struct GlobalLockStream : public OutputStream
    {
	void write( const Issue & issue );
        
      private:
	static std::mutex mutex_;
    };
}

#endif
