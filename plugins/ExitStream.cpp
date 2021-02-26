/*
 *  ExitStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/ExitStream.hpp>
#include <stdlib.h>

ERS_REGISTER_OUTPUT_STREAM( ers::ExitStream, "exit", exit_code )

ers::ExitStream::ExitStream( const std::string & exit_code )
  : m_exit_code( 1 )
{
    std::istringstream in( exit_code );
    in >> m_exit_code;
}

void ers::ExitStream::write( const Issue & )
{
    ::exit( m_exit_code );
}

