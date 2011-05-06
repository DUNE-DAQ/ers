/*
 *  AbortStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/AbortStream.h>
#include <stdlib.h>

ERS_REGISTER_OUTPUT_STREAM( ers::AbortStream, "abort", ERS_EMPTY)

void ers::AbortStream::write( const Issue & issue )
{
    chained().write( issue );
    ::abort();
}




