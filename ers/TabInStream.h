/*
 *  TabInStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 17.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_TAB_IN
#define ERS_TAB_IN

#include "ers/STLInStream.h"

namespace ers {    
    
  /** Simple stream that reads data as tab separated entries
    * \author Matthias Wiesmann
    * \version 1.0
    */
    
    class TabInStream : public STLInStream {
	
protected:
	bool read_line(std::string &key, std::string &value) ;  /**< \brief reads one line and parses key and value */
public: 
	virtual bool read_properties(string_map_type *values) ; /**< \brief loads key and values into a value_table */
	TabInStream(const char* filename) ;                     /**< \brief Builds a Tab format stream that reads from a file */
	TabInStream(const std::string &filename) ;
	TabInStream(const System::File &file); 
	TabInStream();                                          /**< \brief Builds a Tab format stream that reads from stdin */
    } ; // TabInStream
    
} // ers 

#endif
