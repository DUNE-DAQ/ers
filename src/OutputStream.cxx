/*
 *  OutputStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/OutputStream.h>
#include <ers/internal/NullStream.h>


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
