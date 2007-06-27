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
  * This file defines the ers::Context interface.
  */ 

#include <string>
#include <vector>

namespace ers
{   
    /** This class provides an abstract interface for the Context information of an issue.
      * The implementation might be different depending on wether the current context is
      * local. i.e. produced in the current process or it was received as part of the
      * issue, which has been cretaed in another process.
      *
      * \author Serguei Kolos
      * \brief Context interface for Issue.
      */
    class Context
    {
      public:

	static const Context EmptyInstance;

	std::string position() const;				/**< \return position in the code */
	std::vector<std::string> stack( ) const;		/**< \return stack frames vector */
	
        virtual Context * clone() const = 0;			/**< \return copy of the current context */
        virtual const char * const cwd() const = 0;		/**< \return current working directory of the process */
        virtual const char * const file_name() const = 0;	/**< \return name of the file which created the issue */
        virtual const char * const function_name() const = 0;	/**< \return name of the function which created the issue */
        virtual const char * const host_name() const = 0;	/**< \return host where the process is running */
        virtual int line_number() const = 0;			/**< \return line number, in which the issue has been created */
        virtual const char * const package_name() const = 0;	/**< \return CMT package name */
        virtual pid_t process_id() const = 0;			/**< \return process id */
        virtual pid_t thread_id() const = 0;			/**< \return thread id */
        virtual void * const * stack_symbols() const = 0;	/**< \return stack frames */
        virtual int stack_size() const = 0;			/**< \return number of frames in stack */
        virtual int user_id() const = 0;			/**< \return user id */
        virtual const char * const user_name() const = 0;	/**< \return user name */
    };
}

#endif

