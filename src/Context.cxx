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
    position_s << file_name << ":" << line_number << "(" << function_name << ")" ; 
    return position_s.str();
} // position

std::string ers::Context::compiler() const {
    std::ostringstream compiler_s ;
    compiler_s << compiler_name << " " << compiler_version ; 
    return compiler_s.str(); 
} // compiler

std::string ers::Context::compilation() const {
    std::ostringstream compilation_s ;
    compilation_s << compilation_time << " " << compilation_date ; 
    return compilation_s.str(); 
} // compilation

std::string ers::Context::host_type() const {
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


    


