/*
 *  Context.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_CONTEXT_
#define ERS_CONTEXT_

#include <string>

namespace ers {

/** This class encapsulates Context information for an issue.
  * The context of an issue is the location in the code the issue was constructed. 
  * The context acts as an encapsulator for compilator generted information like:
  * - source file
  * - source line
  * - function name
  * - compilation date and time
  * - compilator type 
  *
  * The current context can be obtained using the macro ERS_HERE
  * @author Matthias Wiesmann
  * @version 0.1 
  */
 
class Context {
protected:
        std::string file_name ; 
        int line_number ; 
        std::string function_name ; 
        std::string compiler_name ; 
        std::string compiler_version ; 
        std::string compilation_date ; 
        std::string compilation_time ; 
public:
            Context(const std::string &filename, int line_number, const std::string &function_name, 
                    const std::string &compiler_name, const std::string &compiler_version, 
                    const std::string &compilation_time, const std::string &compilation_date) ;
        std::string file() const ;
        int line() const ;
	const char* function() const ; 
        std::string position() const ;
        std::string compiler() const ; 
        std::string compilation() const ; 
        std::string host_type() const ; 
} ; // Context

} // ers 

#ifdef __GNUC__
#define COMPILER_NAME "gcc"
#endif

#ifdef __INTEL_COMPILER
#define COMPILER_NAME "icc" 
#endif

#ifndef COMPILER_NAME
#define COMPILER_NAME "unknown"
#endif

#ifndef __VERSION__
#define __VERSION__ "unknown"
#endif

/** \def ERS_HERE This macro constructs a context object with all the current values 
  * If gnuc is present, we use the pretty function name instead of the standard __func__ name. 
  */ 

#ifdef __GNUC__
#define ERS_HERE ers::Context(__FILE__,__LINE__,__PRETTY_FUNCTION__,COMPILER_NAME,__VERSION__,__TIME__,__DATE__)
#else
#define ERS_HERE ers::Context(__FILE__,__LINE__,__func__,COMPILER_NAME,__VERSION__,__TIME__,__DATE__)
#endif

#endif

