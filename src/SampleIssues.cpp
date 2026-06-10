/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from src/SampleIssues.cxx to src/SampleIssues.cpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

#include <ers/SampleIssues.hpp>

/** \def ers::File This is the base class for all file related issues. 
 */
ERS_DEFINE_ISSUE_CXX( 	ers, // namespace
			File, // issue class name
			ERS_EMPTY, // no message
                        ((const char *)file_name ) // single attribute 
                 )

/** \def ers::CantOpenFile This issue is reported when a certain file can 
 * not be opened by any reason. 
 */
ERS_DEFINE_ISSUE_BASE_CXX( 	ers, // namespace
				CantOpenFile, // issue class name
                                ers::File, // base class name
				"Can not open \"" << file_name << "\" file", // message
                                ((const char *)file_name ), // base class attributes
                        	ERS_EMPTY // no attributes in this class
                 	)

/** \def ers::FileDoesNotExist This issue is reported when a certain file does not exist. 
 */
ERS_DEFINE_ISSUE_BASE_CXX( 	ers, // namespace
				FileDoesNotExist, // issue class name
				ers::File, // base class name
				"File \"" << file_name << "\" does not exist", // message
                        	(( const char * )file_name ), // base class attributes
				ERS_EMPTY // no attributes in this class
                 	)

/** \def ers::PermissionDenied This issue is reported when a certain file exists but
 * but not accessible for the current user. 
 */
ERS_DEFINE_ISSUE_BASE_CXX( 	ers, 					// namespace
				PermissionDenied, 			// issue class name
				ers::File, // base class name
				"You are not allowed to open \"" 
                        	<< file_name << "\" file, which has " 
                        	<< mode << " access mode",		// message
                        	((const char *)file_name ),		// base class attributes
				((int)mode )				// attribute of this class
                 	)

