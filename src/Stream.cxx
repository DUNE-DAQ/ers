/*
 *  Stream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 01.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <assert.h>

#include "ers/Issue.h"
#include "ers/TabOutStream.h"
#include "ers/Precondition.h"

using namespace ers ; 

Stream* Stream::_error = 0 ; 

Stream* Stream::error() {
    if (! _error) {
	_error = new TabOutStream(); 
    } // if
    return _error ; 
} // get_default

void Stream::error(const Issue *i) {
    ERS_PRECONDITION(i!=0,"Null Issue sending"); 
    error()->send(i); 
} // send

