/*
 *  LocalContext.h
 *  ers
 *
 *  Created by Serguei Kolos on 26.11.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_LOCAL_CONTEXT_H
#define ERS_LOCAL_CONTEXT_H

/** \file LocalContext.h
  * This file defines the ers::LocalContext class,
  * which implements the ers::Context interface.
  */ 

#include <ers/Context.h>
#include <execinfo.h>

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
    struct LocalCompilerContext
    {
	/** Constructor - defines an Issue Compilation context.
	  * \param name name of the compiler
	  * \param version version of the compiler
	  * \param date date of the compilation
	  * \param time time of the compilation
	  * \param package name of the current CMT package
	  */
	LocalCompilerContext(	const char * const name,
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
    
    struct LocalProcessContext
    {
	LocalProcessContext(	const char * const host_name,
				int pid,
				const char * const cwd,
				int uid,
				const char * const uname )
	  : m_host_name( host_name ),
            m_pid( pid ),
            m_cwd( cwd ),
            m_uid( uid ),
            m_uname( uname )
        { ; }
        
	const char * const	m_host_name;	/**< host name */
	const int 		m_pid;		/**< process id */
	const char * const	m_cwd;		/**< process cwd */
	const int		m_uid;		/**< user id */
	const char * const	m_uname;	/**< user name */
    };
    
    namespace
    {
	//
        // This context might be different for every file
        //
        const LocalCompilerContext g_compiler(	COMPILER_NAME,
						__VERSION__,
						__DATE__,
						__TIME__,
						ERS_PACKAGE );
    }
    
    class LocalContext : public Context
    {
      public:

	/** Constructor - defines a local Issue context.
	  * This constructor should generally not be called directly, instead use the macro \c ERS_HERE.
	  * \param filename name of the source code file
	  * \param line_number line_number in the source code
	  * \param function_name name of the function - either pretty printed or not
	  */
	LocalContext( const char * filename, int line_number, const char * function_name )
	  : m_file_name( filename ),
	    m_function_name( function_name ),
	    m_line_number( line_number ),
#ifndef ERS_NO_DEBUG            
	    m_stack_size( backtrace( m_stack, 128 ) )
#else
	    m_stack_size( 0 )
#endif
	{ ; }

        virtual ~LocalContext()
        { ; }

        virtual Context * clone() const			/**< \return copy of the current context */
        { return new LocalContext( *this ); }
        
        const char * const compiler_name() const	/**< \return compiler name */
        { return g_compiler.m_name; }
        
        const char * const compiler_version() const	/**< \return compiler version */        
        { return g_compiler.m_version; }
        
        const char * const compilation_date() const	/**< \return compilation date */
        { return g_compiler.m_date; }
        
        const char * const compilation_time() const	/**< \return compilation time */
        { return g_compiler.m_time; }
        
        const char * const cwd() const			/**< \return current working directory of the process */
        { return c_process.m_cwd; }
        
        const char * const file_name() const		/**< \return name of the file which created the issue */
        { return m_file_name; }
        
        const char * const function_name() const	/**< \return name of the function which created the issue */
        { return m_function_name; }
        
        const char * const host_name() const		/**< \return host where the process is running */
        { return c_process.m_host_name; }
        
        int line_number() const				/**< \return line number, in which the issue has been created */
        { return m_line_number; }
        
        const char * const package_name() const		/**< \return CMT package name */
        { return g_compiler.m_package; }
        
        int process_id() const				/**< \return process id */
        { return c_process.m_pid; }
        
        void * const *  stack_symbols() const		/**< \return stack frames */
        { return m_stack; }
        
        int stack_size() const				/**< \return number of frames in stack */
        { return m_stack_size; }
        
        int user_id() const				/**< \return user id */
        { return c_process.m_uid; }
        
        const char * const user_name() const		/**< \return user name */
        { return c_process.m_uname; }

      private:
        static const LocalProcessContext	c_process;

        const char * const			m_file_name;	/**< source file-name */
	const char * const			m_function_name;/**< source function name */
	const int				m_line_number;	/**< source line-number */	
        void *					m_stack[128];	/**< stack frames */
	const int				m_stack_size;	/**< stack frames number */
    };
}

/** \def ERS_HERE This macro constructs a context object with all the current values 
  */ 
#define ERS_HERE ers::LocalContext( __FILE__, __LINE__, __PRETTY_FUNCTION__ )

#endif

