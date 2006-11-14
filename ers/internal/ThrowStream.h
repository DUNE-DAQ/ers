/*
 *  ThrowStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

/** \file ThrowStream.h This file defines ThrowStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_THROW_STREAM_H
#define ERS_THROW_STREAM_H

#include <ers/OutputStream.h>

namespace ers
{
    
    class Issue; 

    /** Throw stream.
      * This stream class implements a stream, which throws the issue written to it as a normal C++ exception.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Throws issues as exceptions
      */
    
    struct ThrowStream : public OutputStream
    {
	void write( const Issue & issue );	/**< \brief Sends an issue into the stream */
    };
}

#endif

