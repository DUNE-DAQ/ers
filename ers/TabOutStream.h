/*
 *  STLOutStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_TAB_OUT
#define ERS_TAB_OUT

#include "ers/STLOutStream.h"

namespace ers {

    /** Simple stream that writes data as tab separated entries
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
class TabOutStream : public STLOutStream {
protected:
    void serialize_start(const Issue *i) ; 
    virtual void serialize_end(const Issue *i) ;
    void serialize(const std::string &key, const std::string &value) ; 
public: 
    TabOutStream(const char* filename) ;  /**< \brief Builds a Tab format stream that writes into a file */
    TabOutStream(std::ostream *s) ;       /**< \brief Builds a Tab format stream that writes into a STL strean, */
    TabOutStream(const std::string& filename) ; /**< \brief Builds a Tab format stream that writes into a file */
    TabOutStream(const System::File& file) ; /**< \brief Builds a Tab format stream that writes into a file */
    TabOutStream();                       /**< \brief Builds a Tab format stream that writes onto stderr */
   
} ; // Tab_Stream

} // ers 

#endif

