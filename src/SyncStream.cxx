/*
 *  SyncStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "ers/SyncStream.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"
#include "ers/InvalidReferenceIssue.h"


ers::SyncStream::SyncStream() : Stream() {} 
ers::SyncStream::~SyncStream() {}

/** Sends the issue into the stream.
  * This method should put the issue into a FIFO queue and be non blocking
  * @param i pointer to the issue to send
  * @note This method is not implemented, pending some form of common TDAQ thread library. 
  */

void ers::SyncStream::send(const ers::Issue *i) {
    ERS_PRE_CHECK_PTR(i); 
    NOT_IMPLEMENTED();
} // send

/** Blocking read into the stream. 
  * @return a reference to the next Issue
  * @note This method is not implemented, pending some form of common TDAQ thread library. 
  */

ers::Issue *ers::SyncStream::receive() {
    NOT_IMPLEMENTED();
    return 0 ; 
} // receive
