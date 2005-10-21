/*
 *  DequeStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/Issue.h>
#include <ers/internal/DequeStream.h>

ERS_REGISTER_STREAM( ers::DequeStream, "deque", )

void ers::DequeStream::write( const Issue & issue )
{
    m_deque.push_back( issue.clone() );
}

//
// TODO: This must be a blocking read. Will be implemented when thread library will come up
//
ers::Issue * ers::DequeStream::read( )
{
    ers::Issue * issue = 0;
    if ( !m_deque.empty() )
    {
    	issue = m_deque.front();
        m_deque.pop_front();
    }
    return issue;
}
