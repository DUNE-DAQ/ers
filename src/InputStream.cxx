/*
 *  InputStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/InputStream.h>

ers::InputStream::InputStream( )
{ ; }

void ers::InputStream::receive( const Issue & issue )
{
    m_receiver->receive( issue );
}
