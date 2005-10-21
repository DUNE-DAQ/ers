/*
 *  StandardErrStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <iostream>
#include <ers/internal/StandardErrStream.h>

ERS_REGISTER_STREAM( ers::StandardErrStream, "stderr", )

void ers::StandardErrStream::write( const Issue & issue )
{
    std::cerr << issue << std::endl;
    chained().write( issue );
}




