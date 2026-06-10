/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from src/OutputStream.cxx to src/OutputStream.cpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

/*
 *  OutputStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/OutputStream.hpp>
#include <ers/internal/NullStream.hpp>


ers::OutputStream::OutputStream( )
{ ; }

ers::OutputStream &
ers::OutputStream::chained( )
{
    if ( !m_chained.get() )
    {
    	m_chained.reset( new ers::NullStream() );
    }
    return *m_chained;
}

void
ers::OutputStream::chained( OutputStream * stream )
{
    m_chained.reset( stream );
}

bool
ers::OutputStream::isNull() const
{
    return false;
}
