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


using namespace ers ; 

// --------------
// Member methods 
// --------------

Stream::Stream() {}
Stream::~Stream() {}

/** Sends the issue to the stream.
  * \param i the issue to send 
  * \note This implementation silently discards the Issue
  */

void Stream::send(const Issue *i) {
    (void) i ; 
} // send 

/** Reads an isssue on the stream
  * \return the issue that has been read
  * \note This implementation always returns a null pointer
  */

Issue *Stream::receive() {
    return 0 ; 
} // receive




