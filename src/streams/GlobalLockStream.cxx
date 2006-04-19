/*
 *  LockStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 12.04.06.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/GlobalLockStream.h>

ERS_REGISTER_OUTPUT_STREAM( ers::GlobalLockStream, "glock", )

boost::mutex	ers::GlobalLockStream::mutex_;

void ers::GlobalLockStream::write( const Issue & issue )
{
    boost::mutex::scoped_lock slock( mutex_ );
    chained().write( issue );
}
