/*
 *  Executable.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 06.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/File.h"
#include "ers/Core.h"

namespace ers {
    
    /** This class represents an executable file, that is a program. 
      * The class offers facilities to launch the program with given parameters
      * environnement variables
      */
    
    class Executable : public File {
	
protected:
	static void exec(char** argv); 
public:
	static void setenv(const std::string &key, const std::string &value) ; 
	static void setenv(const string_map_type &envs) ; 
	
	Executable(const char* filename) ; 
	Executable(const std::string &filename) ;
	~Executable() ;
	
	void exec(); 
	void exec(const string_vector &params); 
	void exec(const string_vector &params, const string_map_type &envs) ;
	pid_t start(const string_vector &params); 
	pid_t start(const string_vector &params, const string_map_type &envs) ; 
	
	
    } ; // Executable
    
} // ers

