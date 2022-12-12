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
