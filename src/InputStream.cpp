/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82 (2020-04-14).
 * Renamed since fork: yes (from src/InputStream.cxx to src/InputStream.cpp).
 */

/*
 *  InputStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/InputStream.hpp>

namespace 
{
    struct DummyReceiver: public ers::IssueReceiver
    {
	void receive( const ers::Issue & ) override { ; }
    };
    
    DummyReceiver dummy_receiver;
}
       
ers::InputStream::InputStream( )
  : m_receiver( &dummy_receiver )
{ ; }

void ers::InputStream::receive( const Issue & issue )
{
    m_receiver->receive( issue );
}
