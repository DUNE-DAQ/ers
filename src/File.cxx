/*
 *  File.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <unistd.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "ers/File.h"
#include "ers/GetWDFail.h"
#include "ers/FStatFail.h"
#include "ers/UnlinkFail.h"
#include "ers/RenameFail.h"
#include "ers/ChmodFail.h"
#include "ers/OpenFail.h"
#include "ers/InvalidReferenceIssue.h"

#define READ_CHAR_CODE 'r' 
#define WRITE_CHAR_CODE 'w' 
#define EXECUTE_CHAR_CODE 'x' 
#define NOTHING_CHAR_CODE '-' 
#define SETUID_CHAR_CODE 'S' 

/** \return a string containing the working directory for the process */

std::string ers::File::working_directory() {
    const char *wd_buffer = getcwd(0,MAXPATHLEN);  
    std::string directory = std::string(wd_buffer) ; 
    delete(wd_buffer); 
    if (0==errno) return directory ; 
    throw ERS_GET_WD_ERROR() ; 
} // wd 

/** Builds a prettyfied version of permissions. 
  * This should look similar to what \c ls returns with the -l flag
  * \param permissions the permission to beautify 
  * \return a string containing the beautified permissions 
  */

std::string ers::File::pretty_permissions(mode_t permissions) {
    char buffer[11] ; 
    buffer[0] = (permissions & S_ISUID) ? SETUID_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[1] = (permissions & S_IRUSR) ? READ_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[2] = (permissions & S_IWUSR) ? WRITE_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[3] = (permissions & S_IXUSR) ? EXECUTE_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[4] = (permissions & S_IRGRP) ? READ_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[5] = (permissions & S_IWGRP) ? WRITE_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[6] = (permissions & S_IXGRP) ? EXECUTE_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[7] = (permissions & S_IROTH) ? READ_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[8] = (permissions & S_IWOTH) ? WRITE_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[9] = (permissions & S_IXOTH) ? EXECUTE_CHAR_CODE : NOTHING_CHAR_CODE ; 
    buffer[10] = '\0' ; 
    return std::string(buffer); 
} // pretty_permissions

/** Builds a prettyfied version of flags used by the \c open system call
  * \param flags as passed to \c open
  * \return a string containing the beautified flags
  * \note This method only handles flags available accross plateforms. 
  */

std::string ers::File::pretty_open_flag(int flags) {
    std::ostringstream flag_str ; 
    if (flags & O_WRONLY) {
        flag_str << "WRITE" ; 
    } else if (flags & O_RDWR) {
        flag_str << "READ-WRITE" ; 
    } else {
        flag_str << "READ" ; 
    }
    if (flags & O_NONBLOCK) {
        flag_str << "/NOBLOCK" ; 
    } 
    if (flags & O_APPEND) {
        flag_str << "/APPEND" ; 
    }
    if (flags & O_CREAT) {
        flag_str << "/CREATE" ; 
    }
    if (flags & O_TRUNC) {
        flag_str << "/TRUNCATE" ; 
    } 
    if (flags & O_EXCL) {
        flag_str << "/EXCLUSIVE" ; 
    }
    flag_str << "(" << flags << ")" ; 
    return flag_str.str();
} // pretty_open_permission

/** Constructor 
  * \note This method does not create a file, it simply builds an object describing a file 
  */

ers::File::File(const std::string &name) {
    set_name(name); 
} // File

/** Constructor 
  * \overload
  */

ers::File::File(const char* c_name) {
    ERS_PRE_CHECK_PTR(c_name); 
    std::string name = std::string(c_name);
    set_name(name); 
} // File


/** Sets the name for a file.
  * If the name is relative (does not start with /),
  * the absolute path is build using the working directory 
  * \param name the name of the file
  */

void ers::File::set_name(const std::string &name) {
    if (name.substr(0,1)=="/") { // absolute path
	m_full_name = name ; 
    } else {
	m_full_name = working_directory() + name ; 
    }
} // set_name

/** \return the full (absolute) path of the file */

std::string ers::File::full_name() const {
    return m_full_name.c_str() ; 
} // full_name

/** \return the short name of the file - that is the name of the file in its directory */

std::string ers::File::short_name() const {
    std::string::size_type slash = m_full_name.rfind('/');
    return m_full_name.substr(slash); 
} // short_name

/** \return the path of the directory containing the file */

std::string ers::File::directory() const {
    std::string::size_type slash = m_full_name.rfind('/');
    return m_full_name.substr(0,slash); 
} // directory

/** \return the extension of the file */

std::string ers::File::extension() const {
    std::string::size_type dot = m_full_name.rfind('.');
    return m_full_name.substr(dot) ; 
} // extension

/** Checks if the file exists in the filesystem
  * \return \c true if the file exists, false otherwise 
  * \exception ers::IOIssue if an error occurs 
  */

bool ers::File::exists() const {
    struct stat file_status ; 
    const int result = stat(m_full_name.c_str(),&file_status);
    if (0==result) return true ; 
    if (ENOENT==errno) return false ; 
    throw ERS_FSTAT_ERROR(m_full_name.c_str()) ; 
} // exists

/** \return the permissions for the file
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */

mode_t ers::File::permissions() const {
    struct stat file_status ; 
    const int result = stat(m_full_name.c_str(),&file_status);
    if (0==result) return file_status.st_mode ;
    throw ERS_FSTAT_ERROR(m_full_name.c_str());
} // permissions

/** \return the prettfied permissions for the file
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */

std::string ers::File::pretty_permissions() const {
    return pretty_permissions(permissions()); 
} // pretty_permissions

/** \return the size (in bytes) of the file
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */

size_t ers::File::size() const {
    struct stat file_status ; 
    const int result = stat(m_full_name.c_str(),&file_status);
    if (0==result) return file_status.st_size ;
    throw ERS_FSTAT_ERROR(m_full_name.c_str());
} // size 

/** Unlinks (i.e deletes) a file. 
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */

void ers::File::unlink() {
    const int result = ::unlink(m_full_name.c_str()) ; 
    if (0==result) return ; 
    throw ERS_UNLINK_ERROR(m_full_name.c_str()); 
} //unlink

/** Renames or moves a file. 
  * \param new_name the new name of the file 
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */

void ers::File::rename(const std::string &new_name) {
    const char *source = m_full_name.c_str() ; 
    const char *dest = new_name.c_str() ; 
    const int result = ::rename(source,dest); 
    if (0==result) return ;
    throw ERS_RENAME_ERROR(source,dest) ; 
} // rename

/** Sets the permissions of the file 
  * \param permissions the new permissions
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */
 
void ers::File::permissions(mode_t permissions) {
    const int result = ::chmod(m_full_name.c_str(),permissions); 
    if (0==result) return ; 
    throw ERS_CHMOD_ERROR(m_full_name.c_str(),permissions) ; 
} // permissions

/** Conversion into a input stream pointer
  * This actually creates a new input stream that reads from the file. 
  * \return a dynamically allocated input stream 
  * \exception ers::IOIssue if an error occurs or the file does not exist 
  */

ers::File::operator std::istream*() const {
    try {
	std::ifstream *stream = new std::ifstream(m_full_name.c_str()) ; 
	stream->exceptions(std::ios::failbit | std::ios::badbit); 
	return stream ; 
    } catch (std::ios_base::failure &ex) {
	throw ERS_OPEN_READ_FAIL(m_full_name,&ex); 
    } // catch
} // std::istream*

/** Conversion into an output stream pointer
 * This actually creates a new output stream that writes to the file. 
 * \return a dynamically allocated output stream 
 * \exception ers::IOIssue if an error occurs or the file does not exist 
 */
    
ers::File::operator std::ostream*() const {
    try {
	std::ofstream *stream = new std::ofstream(m_full_name.c_str()) ; 
	stream->exceptions(std::ios::failbit | std::ios::badbit); 
	return stream ; 
    } catch (std::ios_base::failure &ex) {
	throw ERS_OPEN_WRITE_FAIL(m_full_name,&ex); 
    } // catch
} // std::ostream*

/** Conversion into a string
  * \return the full name of the file 
  * \see full_name()
  */

ers::File::operator std::string() const {
    return full_name() ;
} // std::string



