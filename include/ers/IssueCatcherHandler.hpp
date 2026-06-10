/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from ers/IssueCatcherHandler.h to include/ers/IssueCatcherHandler.hpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef ERS_ISSUE_CATCHER_HANDLER_H
#define ERS_ISSUE_CATCHER_HANDLER_H

/*
 *  IssueCatcherHandler.h
 *  ers
 *
 *  Created by Serguei Kolos on 21.11.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file IssueCatcherHandler.h This file declares utility class which is used to unregister IssueCatcher.
  * \author Serguei Kolos
  * \brief ers header and documentation file
  */
namespace ers
{
    class LocalStream;
    
    /**
     * This is a helper class that is used to support issue catcher management. An instance of this class
     * holds a reference to the last successfully registered issue catcher. When this instance is destroyed
     * the issue catcher is unregistered.
     *
     * \author Serguei Kolos
     * \brief Implements issue catcher lifetime management.
     */
    class IssueCatcherHandler
    {
	friend class LocalStream;
        
      public:	
	~IssueCatcherHandler();
        
      private:
	IssueCatcherHandler() = default;

	IssueCatcherHandler (const IssueCatcherHandler &) = delete;
	IssueCatcherHandler & operator = (const IssueCatcherHandler &) = delete;
    };
}

#endif
