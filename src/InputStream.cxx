/*
 *  InputStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/InputStream.h>

namespace 
{
    struct DummyReceiver: public ers::IssueReceiver
    {
	void receive( const ers::Issue & ) { ; }
    };
    
    DummyReceiver __dummy__receiver__;
}
       
ers::InputStream::InputStream( )
  : m_receiver( &__dummy__receiver__ )
{ ; }

void ers::InputStream::receive( const Issue & issue )
{
    m_receiver->receive( issue );
}
