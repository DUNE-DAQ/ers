/*
 *  LockStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 12.04.06.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/LockStream.hpp>

ERS_REGISTER_OUTPUT_STREAM( ers::LockStream, "lock", ERS_EMPTY)

void ers::LockStream::write( const Issue & issue )
{
    std::scoped_lock slock( m_mutex );
    chained().write( issue );
}
