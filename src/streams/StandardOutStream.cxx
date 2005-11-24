/*
 *  StandardOutStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <iostream>
#include <ers/internal/StandardOutStream.h>

ERS_REGISTER_OUTPUT_STREAM( ers::StandardOutStream, "stdout", )

void ers::StandardOutStream::write( const Issue & issue )
{
    std::cout << issue << std::endl;
    chained().write( issue );
}
