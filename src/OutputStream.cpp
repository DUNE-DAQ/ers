/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82 (2020-04-14).
 * Renamed since fork: yes (from src/OutputStream.cxx to src/OutputStream.cpp).
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
