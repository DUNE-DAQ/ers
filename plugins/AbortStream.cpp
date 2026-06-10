/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from src/streams/AbortStream.cxx to plugins/AbortStream.cpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

/*
 *  AbortStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/AbortStream.hpp>
#include <stdlib.h>

ERS_REGISTER_OUTPUT_STREAM( ers::AbortStream, "abort", ERS_EMPTY)

void ers::AbortStream::write( const Issue & issue )
{
    chained().write( issue );
    ::abort();
}




