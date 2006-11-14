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
#include <boost/thread/mutex.hpp>

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
	static boost::mutex	mutex_;
    };
}

#endif
