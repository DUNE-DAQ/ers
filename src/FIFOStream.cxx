/*
 *  FIFOStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "ers/FIFOStream.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"
#include "ers/InvalidReferenceIssue.h"


ers::FIFOStream::FIFOStream() : Stream() {} 
ers::FIFOStream::~FIFOStream() {}

/** Sends the issue into the stream.
  * This method should put the issue into a FIFO queue and be non blocking
  * @param i pointer to the issue to send
  * @note This method is not implemented, pending some form of common TDAQ thread library. 
  */

void ers::FIFOStream::send(const ers::Issue *i) {
    ERS_PRE_CHECK_PTR(i); 
    Issue *cloned = i->clone();
    m_issue_queue.push_back(cloned); 
} // send

/** Blocking read into the stream. 
  * @return a reference to the next Issue
  * @note This method is not implemented, pending some form of common TDAQ thread library. 
  */

ers::Issue *ers::FIFOStream::receive() {
    if (m_issue_queue.empty()) return 0 ; 
    Issue *issue = m_issue_queue[0] ; 
    m_issue_queue.pop_front();
    return issue ; 
} // receive


