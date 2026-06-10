/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from ers/internal/NullStream.h to include/ers/internal/NullStream.hpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

/*
 *  NullStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

/** \file NullStream.h This file defines NullStream ERS stream.
  * \author Serguei Kolos
  * \brief ers header file
  */

#ifndef ERS_NULL_STREAM_H
#define ERS_NULL_STREAM_H

#include <ers/OutputStream.hpp>
#include <ers/InputStream.hpp>

namespace ers
{
    class Issue;

    /** This stream implementation silently discards any issue that is sent to it.
     * In order to employ this implementation in a stream configuration the name to be used is "null".
     * E.g. the following configuration will result in no output been produced for the ERROR stream:
     *
     *         export DUNEDAQ_ERS_ERROR="null"
     *
     * \author Serguei Kolos
     * \brief Null stream
     */

    struct NullStream : public OutputStream
    {
        void write( const Issue & ) override
        { ; }

        bool isNull() const override
        { return true; }
    };
}

#endif

