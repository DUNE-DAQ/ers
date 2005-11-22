/*
 *  Context.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Modified by Serguei Kolos on 26.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_CONTEXT_H
#define ERS_CONTEXT_H

/** \file Context.h
  * This file defines the ers::Context object, and the associated macros, like ERS_HERE.
  */ 

#include <string>
#include <vector>

#include <execinfo.h>

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

#ifdef TDAQ_PACKAGE_NAME
#define ERS_PACKAGE TDAQ_PACKAGE_NAME
#else
#define ERS_PACKAGE "unknown"
#endif

namespace ers
{   
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
      * \author Matthias Wiesmann, Serguei Kolos
      * \brief Source context for Issue.
      */
    
    struct CompilerContext
    {
	CompilerContext( const char * const name,
        		 const char * const version,
			 const char * const date,
                         const char * const time,
                         const char * const package )
          : m_name( name ),
            m_version( version ),
            m_date( date ),
            m_time( time ),
            m_package( package )
        { ; }
        
        const char * const m_name;	/**< compiler name */
	const char * const m_version;	/**< compiler version */
	const char * const m_date;	/**< compilation date */
	const char * const m_time;	/**< compilation time */
	const char * const m_package;	/**< CMT package name */
    };
    
    struct ProcessContext
    {
	ProcessContext(	const char * const host_name,
			int pid,
			const char * const cwd,
			int uid,
			const char * const uname )
	  : m_host_name( host_name ),
            m_process_id( pid ),
            m_cwd( cwd ),
            m_user_id( uid ),
            m_user_name( uname )
        { ; }
        
	const char * const	m_host_name;	/**< host name */
	const int 		m_process_id;	/**< process id */
	const char * const	m_cwd;		/**< process cwd */
	const int		m_user_id;	/**< user id */
	const char * const	m_user_name;	/**< user name */
    };
    
    namespace
    {
	//
        // This context might be different for every file
        //
        const CompilerContext compiler_context(	COMPILER_NAME,
						__VERSION__,
						__DATE__,
						__TIME__,
						ERS_PACKAGE );
    }
    
    class Context
    {
      public:

	static const Context EmptyInstance;

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
	Context( const char * filename, int line_number, const char * function_name )
	  : m_compiler( compiler_context ),
	    m_file_name( filename ),
	    m_function_name( function_name ),
	    m_line_number( line_number ),
#ifndef NDEBUG            
	    m_stack_size( backtrace( m_stack, 128 ) )
#else
	    m_stack_size( 0 )
#endif
	{ ; }

	std::string position() const;
	std::string compiler() const;
	std::string compiled_at() const;
	std::vector<std::string> stack( ) const;	/**< \return stack frames vector*/

	const CompilerContext	m_compiler;		/**< compiler information */
        const char * const	m_file_name;		/**< source file-name */
	const char * const	m_function_name;	/**< source function name (can be pretty printed or not) */
	const int		m_line_number;		/**< source line-number */
	
        static const ProcessContext	c_process;

      private:
	Context()
	  : m_compiler( compiler_context ),
	    m_file_name( "" ),
	    m_function_name( "" ),
	    m_line_number( -1 ),
	    m_stack_size( 0 )
	{ ; }
        
        void *		m_stack[128];	/** stack frames */
	const int	m_stack_size;	/** stack frames number */
    }; // Context

} // ers 

/** \def ERS_EMPTY macro to an empty context object used when no context is available */
#define ERS_EMPTY (ers::Context::EmptyInstance) 

/** \def ERS_HERE This macro constructs a context object with all the current values 
  */ 
#define ERS_HERE ers::Context(__FILE__,__LINE__,__PRETTY_FUNCTION__)

#endif

