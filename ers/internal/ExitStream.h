/*
 *  ExitStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

/** \file ExitStream.h This file defines ExitStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_EXIT_STREAM_H
#define ERS_EXIT_STREAM_H

#include <ers/OutputStream.h>

namespace ers
{
    
    class Issue; 

    /** Exit stream.
      * This stream class implements a stream, which exits the application if it receives any issue.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Aborts application
      */
    
    struct ExitStream : public OutputStream
    {
	ExitStream( const std::string & exit_code );

	void write( const Issue & issue );	/**< \brief Sends an issue into the stream */

      private:
	int exit_code_;
    };
}

#endif

