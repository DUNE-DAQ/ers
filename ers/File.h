/*
 *  File.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_UTIL_FILE
#define ERS_UTIL_FILE

#include <string>


namespace ers {
    
    /** This class represents a file.
      * it provides tools to manipulate files in a simple way. 
      * all methods throw ers issues in case of error 
      */
    
    class File {
protected:
	std::string m_full_name ;
	void set_name(const std::string &name); 
public:
	static std::string working_directory() ;                      /**< \brief current directory of process */
	static std::string pretty_permissions(mode_t permissions);    /**< \brief pretty prints permissions */
	static std::string pretty_open_flag(int flags) ; 
	static std::string pretty_size(size_t size, bool cut_small); 
	    
	File(const std::string &name); 
	File(const char* name) ; 
	
	std::string full_name() const ;                               /**< \brief full name for file */
	std::string short_name() const ;                              /**< \brief short name for file */
	std::string directory() const ;                               /**< \brief path of directory containing file */
	std::string extension() const ;                               /**< \brief extension for file */
    
	bool exists() const ;                                         /**< \brief does the file exist */
	mode_t permissions() const ;                                  /**< \brief permissions for the file */
	std::string pretty_permissions() const ;                      /**< \brief pretty permissions for the file */
	size_t size() const ;                                         /**< \brief size of file */
	uid_t owner() const ;                                         /**< \brief owner of file */
	gid_t group() const ;                                         /**< \brief group of file */
	bool is_regular() const ;                                     /**< \brief is the file a directory */
	bool is_directory() const ;                                   /**< \brief is the file a regular file */

	void unlink() ;                                               /**< \brief deletes (unlinks) file */
	void rename(const std::string &new_name);                     /**< \brief rename or moves the file */
	void permissions(mode_t permissions);                         /**< \brief sets the type of the file */
	
	std::istream* input() const ;                                 /**< \brief returns an input stream from the file*/
	std::ostream* output() const ;                                /**< \brief returns an output stream to the file*/
	operator std::string() const ;                                /**< \brief converts into a string (path) */
    } ; // File
} // ers

#endif


