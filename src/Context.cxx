/*
 *  Context.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Context.h"
#include <iostream>
#include <sstream>

ers::Context *ers::Context::empty_instance = 0 ; 

ers::Context *ers::Context::empty() {
    if (! empty_instance) {
	empty_instance = new ers::Context("",0,"","","","","") ; 
    }
    return empty_instance ; 
} // empty 

/** Constructor - defines an Issue context.
  * This constructor should generally not be called directly, instead use the macro \c ERS_HERE. 
  * \param filename name of the source code file
  * \param line_number line_number in the source code 
  * \param function_name name of the function - either pretty printed or not
  * \param compiler_name name of the compiler 
  * \param compiler_version version of the compiler 
  * \param compilation_time time of the compilation 
  * \param compilation_date date of the compilation 
  */

ers::Context::Context(const std::string &filename, int line_number, const std::string &function_name, 
                      const std::string &compiler_name, const std::string &compiler_version, 
                      const std::string &compilation_time, const std::string &compilation_date) {
    this->file_name = filename ; 
    this->line_number = line_number ; 
    this->function_name = function_name ; 
    this->compiler_name = compiler_name ; 
    this->compiler_version = compiler_version ; 
    this->compilation_date = compilation_date ; 
    this->compilation_time = compilation_time ; 
} // Context

std::string ers::Context::file() const {
    return this->file_name ; 
} // file

int ers::Context::line() const {
    return this->line_number ; 
} // line 

const char* ers::Context::function() const {
    return this->function_name.c_str() ; 
} // function


std::string ers::Context::position() const {
    std::ostringstream position_s ;
    if (! file_name.empty()) {
	position_s << file_name << ":" << line_number << " ";
    } 
    if (! function_name.empty()) {
	position_s << "(" << function_name << ")" ;
    } // if
    return position_s.str();
} // position

std::string ers::Context::compiler() const {
    std::ostringstream compiler_s ;
    if (! compiler_name.empty()) {
	compiler_s << compiler_name << " " << compiler_version ; 
    } // if 
    return compiler_s.str(); 
} // compiler

std::string ers::Context::compilation() const {
    std::ostringstream compilation_s ;
    if (! compilation_time.empty()) {
	compilation_s << compilation_time << " " ; 
    } // compilation time
    if (! compilation_date.empty()) {
	compilation_s << compilation_date ; 
    } // compilation date
    return compilation_s.str(); 
} // compilation

/** Tries to gues the host name 
  * \return a string describing the host, in the form <var>architecture</var>-<var>operating-system</var>
  */

std::string ers::Context::host_type() {
    std::ostringstream plateform_s ;
#ifdef __POWERPC__
    plateform_s << "powerpc" ; 
#endif
#ifdef __i386__
    plateform_s << "i386" ;
#endif
#ifdef __sparc__
    plateform_s << "sparc" ; 
#endif
    plateform_s << "-" ; 
#ifdef __linux__ 
    plateform_s << "linux" ; 
#endif
#ifdef __MACH__
    plateform_s << "mach" ; 
#endif
#ifdef __SUNPRO_C
    plateform_s << "solaris" ;
#endif    
    return plateform_s.str();
} // plateform


    


