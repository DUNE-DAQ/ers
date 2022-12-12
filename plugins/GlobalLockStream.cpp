/*
 *  LockStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 12.04.06.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/internal/GlobalLockStream.hpp>

ERS_REGISTER_OUTPUT_STREAM(ers::GlobalLockStream, "glock", ERS_EMPTY)

std::mutex ers::GlobalLockStream::mutex_;

void
ers::GlobalLockStream::write(const Issue& issue)
{
  std::scoped_lock slock(mutex_);
  chained().write(issue);
}
