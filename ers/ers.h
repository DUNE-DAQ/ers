/*
 *  ers.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file ers.h This file includes all the main headers of the Error Reporting System.
  * It does not declare anything <em>per se</em>
  * \author Matthias Wiesmann
  * \version 1.1 Removed dependency on System package
  */

#include "ers/Core.h"
#include "ers/Context.h"

#include "ers/Issue.h"
#include "ers/IssueFactory.h"
#include "ers/Stream.h"
#include "ers/StreamFactory.h"

#include "ers/Assertion.h"
#include "ers/Precondition.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/NotImplemented.h"
#include "ers/LogIssue.h"
#include "ers/RangeIssue.h"

/** \page ERSHowTo How to use the ERS package
  This package can be used at different levels. At the lowest level, one can simply use the existing checking macros. 
  A more advanced way of using this package is to define specific Issue subclasses. 
  \section Macros Basic macros
  \subsection AssertionMacros Assertion Macros
  The ERS package offers a set of macros to do basic checking. 
  Those macros generally behave like the standard C assert macros. 
  They have the following features:<ul>
  <li>Different types of checks:
      <ul>
      <li>Compile time checks (\c ERS_STATIC_ASSERT)</li>
      <li>Preconditions (\c ERS_PRECONDITION)</li>
      <li>Pointer precondition (\c ERS_PRE_CHECK_PTR)</li>
      <li>Pointer checking (\c ERS_CHECK_PTR)</li>
      <li>General assertion (\c ERS_ASSERT)</li>
      </ul></li>
  <li>Macros are compiled out if macro \c N_DEBUG is defined</li>
  <li>Throws complete ERS issues, they can be caught streamed, modified etc.</li>
  <li>Preconditions and general assertions support variable number of parameters and \c fprintf like formatting</li>
  <li>Runtime assertion can detect if they are invariant conditions</li>
  </ul>
  All macros throw Issues object as exceptions with a severity_t of \c ers::error.
  \subsection LoggingMacros Logging Macros
  The ERS package offers a set of macros to do logging. Those macros construct an issue and send them to the relevant stream. 
  They all support multiple number of parameters with \c fprintf like patterns.
  For each debug and warning severity_t level there is an associated macro:
  <ul>
  <li>ERS_DEBUG_0</li>
  <li>ERS_DEBUG_1</li>
  <li>ERS_DEBUG_2</li>
  <li>ERS_DEBUG_3</li>
  <li>ERS_WARN</li>
  </ul>
  Each macro can be disabled at compilation time. It is also possible to set up the associated stream to discard messages or to 
  filter them based on qualifiers. 

  \section Issues Building Custom Issues
  For a discussion about building custom issue, see the documentation for the Issue class. 
  An example custom issue is also given in the example directory. 
  \see ers::Issue 
  \see ExampleIssue 

  \section Selecting Streams
  The ERS system use the abstraction of streams to handle issues. 
  Conceptualy, a stream is simply an object that can the user can use to send (or receive) Issues. 
  There is one stream associated with each severity level,
  those streams can be set or retrieved using methods of the ers::StreamFactory class. 
  \see ers::StreamFactory 

*/
