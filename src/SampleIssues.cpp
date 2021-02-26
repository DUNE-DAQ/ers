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

