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
#include "ers/InvalidReferenceIssue.h"

using namespace ers ; 

Stream* Stream::_error = 0 ; 

Stream* Stream::error() {
    if (! _error) {
	_error = new TabOutStream(); 
    } // if
    return _error ; 
} // get_default

void Stream::error(const Issue *i) {
    ERS_PRE_CHECK_PTR(i); 
    error()->send(i); 
} // send

