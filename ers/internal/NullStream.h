/*
 *  NullStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_NULL_STREAM_H
#define ERS_NULL_STREAM_H

#include <ers/OutputStream.h>
#include <ers/InputStream.h>

namespace ers
{
    
    class Issue;

    /** Null stream.
      * This stream class implements a null stream, i.e a stream where no issue can be read from
      * and it also silently discards sent issues.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Null stream
      */
    
    struct NullStream : public OutputStream
    {        	
        void write( const Issue & )	/**< \brief Sends an issue into the stream */
        { ; }
    };
}

#endif

