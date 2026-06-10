/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14T15:33:10+02:00).
 * Renamed since fork: yes (from src/IssueCatcherHandler.cxx to src/IssueCatcherHandler.cpp).
 */

/*
 *  IssueCatcherHandler.cxx
 *  ERS
 *
 *  Created by Serguei Kolos on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/IssueCatcherHandler.hpp>
#include <ers/LocalStream.hpp>

ers::IssueCatcherHandler::~IssueCatcherHandler()
{
    LocalStream::instance().remove_issue_catcher();
}
