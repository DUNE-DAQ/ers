/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from ers/internal/GlobalLockStream.h to include/ers/internal/GlobalLockStream.hpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

/*
 *  LockStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 12.04.06.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
 
/** \file LockStream.h This file defines LockStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_GLOBAL_LOCK_STREAM_H
#define ERS_GLOBAL_LOCK_STREAM_H

#include <mutex>

#include <ers/OutputStream.hpp>

namespace ers
{

    /** This class can be used to protect output produced by distinct ERS streams, e.g. INFO and LOG,
      * from been mixed up when originated from concurrent threads. The name to be used for this stream
      * in stream configurations is "glock".
      * \author Serguei Kolos
      * \brief Lock for ERS streams.
      */

    struct GlobalLockStream : public OutputStream
    {
	void write( const Issue & issue ) override;
        
      private:
	static std::mutex mutex_;
    };
}

#endif
