/*
 *  Executable.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include "ers/Executable.h"
#include "ers/PosixIssue.h"
#include "ers/Precondition.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/ExecFail.h"
#include "ers/ForkFail.h"
#include "ers/AllocIssue.h"

/** Sets up a pair of key/values as an environnement variable 
  */

void ers::Executable::setenv(const std::string &key, const std::string &value) {
    const char* c_key = key.c_str();
    const char* c_value = value.c_str();
    const int status = ::setenv(c_key,c_value,1); 
    if (status<0) throw PosixIssue(ERS_HERE,ers::ers_error,"unable to set environnement") ; 
} // setenv

/** Sets up a set of environnement variables 
  * @param envs the variables to set up 
  */

void ers::Executable::setenv(const string_map_type &envs) {
    for(string_map_type::const_iterator pos = envs.begin();pos!=envs.end();++pos) {
	setenv(pos->first,pos->second); 
    } // for
} // setenv

ers::Executable::Executable(const char* filename) : ers::File(filename) {}
ers::Executable::Executable(const std::string &filename) : ers::File(filename) {}
ers::Executable::~Executable() {}


/** Core execution method. 
  * This method assumes the datastructures have been setup correctly for \c execv.
  * It basically adds some error handling to \c execv
  * \param argv array of arguments, with name of the executable in first position
  * \note this method should not be called directly 
  */

void ers::Executable::exec(char** argv) {
    ERS_PRE_CHECK_PTR(argv);
    ERS_PRE_CHECK_PTR(argv[0]);
    const int status = ::execv(argv[0],argv) ; 
    ERS_ASSERT(status<0,"exec returned status is %d (should be negative)",status);
    throw ERS_EXEC_ERROR(argv[0]) ; ; 
} // exec

/** Simple execution method. 
  * This methods converts the vector of strings into the correct data structures for \execv. 
  * The \c argv structure is allocated dynamically and new string copied into it. 
  */

void ers::Executable::exec(const string_vector &params) {
    const int argc = params.size(); // number of parameters
    const int argclen = argc+2 ;    // size of array parameters + program name + null pointer
    char **argv = (char **) calloc(sizeof(char*),argclen) ; 
    ERS_ALLOC_CHECK(argv,sizeof(char*),argclen) ;
    const char* name = full_name().c_str() ; 
    argv[0] = strdup(name);
    ERS_ALLOC_CHECK(argv[0],strlen(name)) ;
    for(int i=0;i<argc;i++) {
	argv[i+1] = strdup(params[i].c_str()) ; 
    } // loop over args
    argv[argc+1] = 0 ; 
    try {
	exec(argv); 
    } catch (Issue i) { // there was a problem so we deallocate the argc array. 
	for(int i=0;argv[i]!=0;i++) {
	    free(argv[i]); 
	} // for
	free(argv);
	throw ; 
    } // catch 
} // exec 

/** This execution method first sets the environnement variables then calls exec with the parameters
  * \param params the parameters to use when launching the executable
  * \param envs table of the environnement variables set for the executable
  */

void ers::Executable::exec(const string_vector &params, const string_map_type &envs) {
    setenv(envs); 
    exec(params); 
} // exec


/** This method starts the executable in another process using \c fork 
* \param params the parameters to use when launching the executable
* \param envs table of the environnement variables set for the executable
*/

pid_t ers::Executable::start(const string_vector &params) {
    const pid_t child = fork() ;
    if (child == 0) { // we are the child 
	exec(params); 
    } // we are the child 
    if (child > 0) { // we are the parent
	return child ; 
    } // we are the parent
    throw ERS_FORK_ERROR() ; // We are screwed up 
} // start

/** This method starts the executable in another process using \c fork 
  * \param params the parameters to use when launching the executable
  * \param envs table of the environnement variables set for the executable
  */

pid_t ers::Executable::start(const string_vector &params, const string_map_type &envs) {
    const pid_t child = fork() ;
    if (child == 0) { // we are the child 
	exec(params,envs); 
    } // we are the child 
    if (child > 0) { // we are the parent
	return child ; 
    } // we are the parent
    throw ERS_FORK_ERROR() ; // We are screwed up 
} // start



