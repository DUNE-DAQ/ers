/*
 *  ers.h
 *  ERS
 *
 *  Created by Matthias Wiesmann on 26.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file ers.h This file includes all the main headers of the Error Reporting System.
  * It does not declare anything <em>per se</em>
  * \author Matthias Wiesmann
  * \version 1.0
  */

#include "ers/Core.h"
#include "ers/Context.h"
#include "ers/IssueFactory.h"
#include "ers/StreamFactory.h"

#include "ers/Stream.h"

#include "ers/Issue.h"
#include "ers/Assertion.h"
#include "ers/Precondition.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/NotImplemented.h"
