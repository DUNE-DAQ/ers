/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82 (2020-04-14).
 * Renamed since fork: yes (from ers/StandardStreamOutput.h to include/ers/StandardStreamOutput.hpp).
 */

/*
 *  StandardStreamOutput.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.07.
 *  Copyright 2007 CERN. All rights reserved.
 *
 */
 
/** \file StandardStreamOutput.h This file defines C++ standard stream output function for the ERS streams.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_STANDARD_STREAM_OUTPUT_H
#define ERS_STANDARD_STREAM_OUTPUT_H

#include <iostream>

namespace ers
{
    class Issue;
    
    /** This class provides a namespace for the functions that can be used to print ERS issues to a standard C++ output stream.
      * \author Serguei Kolos
      * \version 1.0
      */
  
    struct StandardStreamOutput
    {
        static std::ostream & print( std::ostream & out, const Issue & issue, int verbosity );
        static std::ostream & println( std::ostream & out, const Issue & issue, int verbosity );
    };
}
    
#endif
