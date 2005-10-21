/*
 *  StandardErrStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
 
#ifndef ERS_STANDARD_ERR_STREAM_H
#define ERS_STANDARD_ERR_STREAM_H

#include <ers/Stream.h>

namespace ers
{

    /** This class streams an issue into standard C++ output stream.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Single line, human readable format stream.
      */

    struct StandardErrStream : public Stream
    {
        void write( const Issue & ptr );
        	
        Issue * read( )
        { return 0; }
    };
}

#endif
