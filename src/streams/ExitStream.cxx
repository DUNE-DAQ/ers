/*
 *  ExitStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/ExitStream.h>
#include <stdlib.h>

ERS_REGISTER_OUTPUT_STREAM( ers::ExitStream, "exit", exit_code )

ers::ExitStream::ExitStream( const std::string & exit_code )
  : exit_code_( 13 )
{
    std::istringstream in( exit_code );
    in >> exit_code_;
}

void ers::ExitStream::write( const Issue & issue )
{
    ::exit( exit_code_ );
}

