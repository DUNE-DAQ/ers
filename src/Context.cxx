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
std::string ers::Context::s_host_type  ; 

const ers::Context *ers::Context::empty() {
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
    this->m_file_name = filename ; 
    this->m_line_number = line_number ; 
    this->m_function_name = function_name ; 
    this->m_compiler_name = compiler_name ; 
    this->m_compiler_version = compiler_version ; 
    this->m_compilation_date = compilation_date ; 
    this->m_compilation_time = compilation_time ; 
} // Context

/** The source code file name 
  * \return path of the source file 
  */

const std::string & ers::Context::file() const {
    return m_file_name ; 
} // file

/** The line number in the source code 
  * \return line number 
  */

int ers::Context::line() const {
    return m_line_number ; 
} // line 

/** Name of the function containing the context 
  * \return name of the function
  */

const std::string & ers::Context::function() const {
    return m_function_name ; 
} // function


const std::string & ers::Context::position() const {
    if (m_position.empty()) {
	std::ostringstream position_s ;
	if (! m_file_name.empty()) {
	    position_s << m_file_name << ":" << m_line_number << " ";
	} 
	if (! m_function_name.empty()) {
	    position_s << "(" << m_function_name << ")" ;
	} // if
	m_position = position_s.str();
    } // cached version not calculated 
    return m_position ; 
} // position

const std::string & ers::Context::compiler() const {
    if (m_compiler.empty()) {
	if (! m_compiler_name.empty()) {
	    std::ostringstream compiler_s ;
	    compiler_s << m_compiler_name << " " << m_compiler_version ; 
	    m_compiler = compiler_s.str(); 
	} else {
	    m_compiler = "unknown" ; 
	} 
    } // build cache
    return m_compiler ; 
} // compiler

const std::string & ers::Context::compilation() const {
    if (m_compilation.empty()) {
	std::ostringstream compilation_s ;
	if (! m_compilation_time.empty()) {
	    compilation_s << m_compilation_time << " " ; 
	} // compilation time
	if (! m_compilation_date.empty()) {
	    compilation_s << m_compilation_date ; 
	} // compilation date
	m_compilation = compilation_s.str(); 
    } // if
    return m_compilation ; 
} // compilation

void ers::Context::build_host_type() {
    std::ostringstream plateform_s ;
#if defined(__linux__) 
    plateform_s << "linux" ; 
#endif
#if defined(__OpenBSD__)
    plateform_s << "OpenBSD" ;
#endif
#if defined(__FreeBSD__) 
    plateform_s << "FreeBSD" ;
#endif
#if defined(__APPLE__) && defined(__MACH__)
    plateform_s << "Darwin" ; 
#endif
#if defined(__SOLARIS__)
    plateform_s << "Solaris" ;
#endif    
    plateform_s << "/" ; 
#if defined(__POWERPC__)  || defined(__ppc__ ) || defined( __PPC__ ) || defined( powerpc ) || defined( ppc )
    plateform_s << "PowerPC" ; 
#endif
#if defined(__i386__) || defined(__INTEL__) || defined( intel ) || defined( _M_IX86 ) 
    plateform_s << "i386" ;
#endif
#if defined(sparc) || defined(__sparc)   
    plateform_s << "Sparc" ; 
#endif
   s_host_type=  plateform_s.str();
} // build_host_type

/** Tries to gues the host name 
  * \return a string describing the host, in the form <var>architecture</var>-<var>operating-system</var>
  */

std::string & ers::Context::host_type() {
    if (s_host_type.empty()) build_host_type() ; 
    return s_host_type ; 
} // plateform



