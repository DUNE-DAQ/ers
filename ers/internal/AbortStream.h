/*
 *  AbortStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_ABORT_STREAM_H
#define ERS_ABORT_STREAM_H

#include <ers/OutputStream.h>

namespace ers
{
    
    class Issue; 

    /** Abort stream.
      * This stream class implements a stream, which aborts the application if it receives any issue.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Aborts application
      */
    
    struct AbortStream : public OutputStream
    {
	void write( const Issue & issue );	/**< \brief Sends an issue into the stream */
    };
}

#endif

