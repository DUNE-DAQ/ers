/*
 *  StandardErrStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
 
/** \file StandardErrStream.h This file defines StandardErrStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_STANDARD_ERR_STREAM_H
#define ERS_STANDARD_ERR_STREAM_H

#include <ers/OutputStream.h>

namespace ers
{

    /** This class streams an issue into standard C++ file output stream.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Single line, human readable format stream.
      */

    template <class Device>
    struct StandardStream : public OutputStream,
    			    public Device
    {
        using Device::device;
        
        StandardStream( )
        { ; }
        
        StandardStream( const std::string & file_name )
          : Device ( file_name )
        { ; }
        
        void write( const Issue & issue )
	{
	    device().stream() << issue << std::endl;
	    chained().write( issue );
	}
    };
}
    
#endif
