/*
 *  Stream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/Stream.h>
#include <ers/internal/NullStream.h>


ers::Stream::Stream( )
{ ; }

ers::Stream &
ers::Stream::chained( )
{
    if ( !m_chained.get() )
    {
    	m_chained.reset( new ers::NullStream() );
    }
    return *m_chained;
}

void
ers::Stream::chained( Stream * stream )
{
    m_chained.reset( stream );
}
