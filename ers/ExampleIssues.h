/*
 *  ExampleIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Modified by Serguei Kolos on 24.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/Issue.h>


/** \file ExampleIssues.h Defines examples of user issues. 
  * \brief Example issues
  * \author Serguei Kolos
  * \version 1.0 
  */

/** \def ers::CantOpenFile This issue is reported when a certain file can 
 * not be opened by any reason. 
 */
ERS_DECLARE_ISSUE( 	ers, // namespace
			CantOpenFile, // issue class name
			"Can not open \"" << file_name << "\" file", // message
                        ((const char *)file_name ) // single attribute 
                 )


/** \def ers::FileDoesNotExist This issue is reported when a certain file does not exist. 
 */
ERS_DECLARE_ISSUE( 	ers, // namespace
			FileDoesNotExist, // issue class name
			"File \"" << file_name << "\" does not exist", // message
                        (( const char * )file_name ) // single attribute
                 )

/** \def ers::FileDoesNotExist This issue is reported when a certain file exists but
 * but not accessible for the current user. 
 */
ERS_DECLARE_ISSUE( 	ers, 					// namespace
			PermissionDenied, 			// issue class name
			"You are not allowed to open \"" 
                        << file_name << "\" file, which has " 
                        << mode << " access mode",		// message
                        ((const char *)file_name )		// first attribute
			((int)mode )				// second attribute
                 )



