/*
 *  FileIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_FILE_ISSUE
#define ERS_FILE_ISSUE

#include "ers/IOIssue.h"

namespace ers {
    
    /** This class represents POSIX file issues.
      * It is basically a Posix Issue with an additional tag containing a file path 
      * \author Matthias Wiesmann
      * \version 1.0 
      */
    
    class FileIssue : public IOIssue {
protected:
	void path(const char* path); /**< \brief sets the path related to the error */
	FileIssue(const Context &c, ers_severity s, const char *path) ;
	FileIssue(const Context &c, ers_severity s, const char *path, const std::exception *cause);
	void setup_file_issue(const std::string &message, const char *path); 
public:
	static const char* PATH_KEY ; 
	static const char* FILE_ISSUE_CLASS ; 
	static const char* FILE_TYPE_KEY ; 
	FileIssue(); 
	FileIssue(const Context &c, ers_severity s, const std::string &message, const char *path) ; 
	FileIssue(const Context &c, ers_severity s, const std::string &message, const char *path, const std::exception *cause) ; 
	virtual const char *get_class_name() const throw();
	std::string path() const ; /**< \brief return the path related to the error */
    } ; // open_fail   
    
} // namespace ers 

#define ERS_FILEERROR(msg,...) ers::FileIssue(ERS_HERE,ers::ers_error,msg,__VA_ARGS__) 

#endif

