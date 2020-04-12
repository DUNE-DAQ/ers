/*
 *  FormattedStandardStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.07.
 *  Copyright 2007 CERN. All rights reserved.
 *
 */
 
/** \file FormattedStandardStream.h This file defines FormattedStandardStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_FORMATTED_STANDARD_STREAM_H
#define ERS_FORMATTED_STANDARD_STREAM_H

#include <map>

#include <ers/OutputStream.h>

namespace ers
{

    /** This class streams an issue into standard C++ file output stream.
      * \author Serguei Kolos
      * \version 1.0
      * \brief Single line, human readable format stream.
      */
    namespace format
    {
        enum Token { Severity, Time, Position, Context, Host, PID, TID, User, CWD, Function, Line, Text, Stack, Cause, Parameters, Qualifiers };
    }
    
    template <class Device>
    struct FormattedStandardStream : public OutputStream,
				     public Device
    {
        using Device::device;
        
        FormattedStandardStream( )
        { ; }
        
        FormattedStandardStream( const std::string & format );
        
        void write( const Issue & issue );
        
      private:
	void report( std::ostream & out, const Issue & issue );
        
	struct Fields : public std::map< std::string, format::Token >
        {
            Fields();
        };
        
        static std::string get_file_name( const std::string & param );
	static std::string get_format( const std::string & param );
                
        static Fields			supported_fields_;
        std::vector<format::Token>	tokens_;
    };
}

#include <ers/internal/FormattedStandardStream.i>
    
#endif
