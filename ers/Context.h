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
#include <vector>

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
  * The current context can be obtained using the macro \c ERS_HERE
  * An empty context can be obtained using the macro \c ERS_EMPTY
  *
  * \author Matthias Wiesmann
  * \version 1.1 - now returns references 
  * \brief Source context for Issue. 
  * \note it should be possible to optimize out the compilation text string, have only one default instance if 
  *       all the code share the same info. This would save some memory space. 
  */
 
class Context {
protected:
    static Context* empty_instance ; 
    static std::string s_host_type ;   /**< host_type */
    std::string m_file_name ;          /**< source file-name */
    int m_line_number ;                /**< source line-number */
    std::string m_function_name ;      /**< source function name (can be pretty printed or not) */
    std::string m_compiler_name ;      /**< compiler name */
    std::string m_compiler_version ;   /**< compiler version */
    std::string m_compilation_date ;   /**< compilation date */
    std::string m_compilation_time ;   /**< compilation time */
    mutable std::string m_compiler ;   /**< compilation string (cache) */
    mutable std::string m_position ;   /**< code position (cache) */
    mutable std::string m_compilation ; /**< compilation (cache) */
    std::vector<std::string> m_stack_frames ;
    static void build_host_type() ;
public:
    static const Context* empty() ;
    static std::string & host_type()  ; /**< \brief type of target host */
    static int debug_level(); 
    Context(const std::string &filename, int line_number, const std::string &function_name, 
	    const std::string &compiler_name, const std::string &compiler_version, 
	    const std::string &compilation_time, const std::string &compilation_date) ;
    const std::string & file() const ;       /**< \return file-name */
    int line() const ;                       /**< \return line-number */
    const std::string & function() const ;   /**< \return function name */
    const std::string & position() const ;   /**< \return position (i.e file+line+function) */
    const std::string & compiler() const ;   /**< \return compiler (compiler-name + compiler-version) */
    const std::string & compilation() const ; /**< \return compilation time and date */
    int stack_frames() const ; 
    const std::string &stack_frame(int i) const ; 
   } ; // Context

} // ers 

/** \def COMPILER_NAME defines the name of the compiler used */
/** \def __VERSION__ defines the version of the compiler used */

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

/** \def ERS_EMPTY macro to an empty context object used when no context is available */
#define ERS_EMPTY *(ers::Context::empty()) 

/** \def ERS_HERE This macro constructs a context object with all the current values 
  * If gnuc is present, we use the pretty function name instead of the standard __func__ name.
  * If macro \c N_DEBUG is defined, an empty context is substituted. 
  */ 

#ifndef N_DEBUG
#ifdef __GNUC__
#define ERS_HERE ers::Context(__FILE__,__LINE__,__PRETTY_FUNCTION__,COMPILER_NAME,__VERSION__,__TIME__,__DATE__)
#else
#define ERS_HERE ers::Context(__FILE__,__LINE__,__func__,COMPILER_NAME,__VERSION__,__TIME__,__DATE__)
#endif
#else
#define ERS_HERE ERS_EMPTY
#endif



#endif

