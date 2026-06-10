/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14T15:33:10+02:00).
 * Renamed since fork: yes (from src/streams/ThrowStream.cxx to plugins/ThrowStream.cpp).
 */

/*
 *  ThrowStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/ThrowStream.hpp>

ERS_REGISTER_OUTPUT_STREAM( ers::ThrowStream, "throw", ERS_EMPTY)

void ers::ThrowStream::write( const Issue & issue )
{
    chained().write( issue );
    issue.raise();
}




