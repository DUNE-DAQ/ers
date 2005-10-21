/*
 *  DequeStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
#ifndef ERS_DEQUE_STREAM_H
#define ERS_DEQUE_STREAM_H

#include <deque>
#include <ers/Stream.h>

namespace ers
{

    class Issue; 

    /** This streams is used to transmit Issues inside a process.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Queue stream 
      * \note This stream is \b not thread safe - it cannot depend on thread library. 
      */
    
    class DequeStream : public Stream
    {
        void write( const Issue & issue );
	
        Issue * read( );

      private:
        std::deque<Issue *> m_deque;
    };
} // ers

#endif	

