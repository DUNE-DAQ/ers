/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from src/streams/ExitStream.cxx to plugins/ExitStream.cpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

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

