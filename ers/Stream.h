/*
 *  Stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#include <string>

#include "ers/util.h"

#ifndef __ERS_ISSUE_STREAM__
#define __ERS_ISSUE_STREAM__

namespace ers {
    
    class Issue ; 

/** Root issue stream.
  * This class is meant to be subclassed in order to implement concrete issue streams. 
  * This class silently discards issues.
  * @author Matthias Wiesmann
  * @version 1.0
  */
    
class Stream {
    friend class Issue ; 
protected:
    static Stream *_error ;
public:
    virtual void send(const Issue *i) = 0;          /**< \brief Sends an issue into the stream */
    virtual Issue *receive() = 0; 
    virtual ~Stream() {} 
    static Stream* error() ; 
    static void error(const Issue *i) ; 
} ; 

} // ers 


#endif

