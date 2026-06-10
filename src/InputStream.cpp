/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from src/InputStream.cxx to src/InputStream.cpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
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
