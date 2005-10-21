/*
 *  ThrowStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_ABORT_STREAM_H
#define ERS_ABORT_STREAM_H

#include <ers/Stream.h>

namespace ers
{
    
    class Issue; 

    /** Throw stream.
      * This stream class implements a stream, which aborts the application if it receives any issue.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Throws issues as exceptions
      */
    
    struct AbortStream : public Stream
    {
	void write( const Issue & issue );		/**< \brief Sends an issue into the stream */

	Issue * read()					/**< \brief Receives an issue from the stream */
        { return 0; }	
    } ;
}

#endif

