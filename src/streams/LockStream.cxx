/*
 *  LockStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 12.04.06.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/LockStream.h>

ERS_REGISTER_OUTPUT_STREAM( ers::LockStream, "lock", )

void ers::LockStream::write( const Issue & issue )
{
    boost::mutex::scoped_lock slock( mutex_ );
    chained().write( issue );
}
