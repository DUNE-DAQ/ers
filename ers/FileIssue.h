/*
 *  FileIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 13.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/PosixIssue.h"

namespace ers {
    
    /** This class represents POSIX file issues.
      * It is basically a Posix with an additional tag containing a file path 
      * @author Matthias Wiesmann
      * @version 0.1 
      */
    
    class FileIssue : public PosixIssue {
protected:
	void path(const char* path); /**< \brief sets the path related to the error */
	FileIssue(const char *path, ers_severity s, const Context &context) ;
	void setup_file_issue(const std::string &message, const char *path); 
public:
	static const char* PATH_KEY ; 
	static const char* FILE_ISSUE_CLASS ; 
	FileIssue(); 
	FileIssue(const std::string &message, const char *path, ers_severity s, const Context &context) ; 
	FileIssue(const std::string &message, const char *path, const std::exception *cause, ers_severity s, const Context &context) ; 
	virtual const char *get_class_name() const throw();
	std::string path() const ; /**< \brief return the path related to the error */
    } ; // open_fail   
    
} // namespace ers 

