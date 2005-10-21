/*
 *  ers.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.01.05.
 *  Modified by Serguei Kolos on 26.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file ers.h This file defines the main functions and macros for ERS.
  * It also includes the main headers of the Error Reporting System and
  * contains the doxygen code to generate the general package documentation. 
  * \author Matthias Wiesmann
  * \version 1.1 Removed dependency on System package
  * \brief ers header and documentation file 
  */

#ifndef ERS_ERS_H
#define ERS_ERS_H

#include <sstream>
#include <ers/StreamFactory.h>
#include <ers/Issue.h>
#include <ers/Assertion.h>
#include <ers/Severity.h>

namespace ers
{
    inline int debug_level( )
    { return StreamFactory::instance().debug_level( ); }
    
    inline void debug( const Issue & issue, int level = debug_level() )
    { StreamFactory::instance().debug( issue, level ); }
    
    inline void error( const Issue & issue )
    { StreamFactory::instance().error( issue ); }
    
    inline void fatal( const Issue & issue )
    { StreamFactory::instance().fatal( issue ); }
    
    inline void info( const Issue & issue )
    { StreamFactory::instance().information( issue ); }
    
    inline int verbosity_level( )
    { return StreamFactory::instance().verbosity_level( ); }

    inline void warning( const Issue & issue )
    { StreamFactory::instance().warning( issue ); }
}

ERS_DECLARE_ISSUE( ers, Info, , )

#ifndef ERS_NO_DEBUG
#define ERS_DEBUG( level, args ) { \
if ( ers::debug_level() >= level ) \
{ \
    std::ostringstream out; \
    out << args; \
    ers::debug( ers::Info( ERS_HERE, out.str() ), level ); \
} }
#else
#define ERS_DEBUG( level, args )
#endif

#define ERS_INFO( args ) \
{ \
    std::ostringstream out; \
    out << args; \
    ers::info( ers::Info( ERS_HERE, out.str() ) ); \
}

#define ERS_WARNING( args ) \
{ \
    std::ostringstream out; \
    out << args; \
    ers::warning( ers::Info( ERS_HERE, out.str() ) ); \
}
    	
#endif

/** \page ERSHowTo How to use the ERS package
  The goal of the Error Reporting System is to offer tool to simplify and unify error 
  handling and error reporting in TDAQ applications. 
  This package can be used at different levels. At the lowest level, one can simply 
  use the existing checking and logging macros. 
  For error reporting one has to define specific Issue subclasses. 
  \section Macros Basic macros
  Basic ERS functionality can be exploited by using simple macros. 
  Those macros are available simply by including the ers/ers.h header file. 
  \subsection AssertionMacros Assertion Macros
  The ERS package offers a set of macros to do basic checking. 
  Those macros generally behave like the standard C assert macros:
      <ul>
      <li>General assertion (\c ERS_ASSERT)
	  this macro can be used for checks that do not fit into any other category. 
      <li>Preconditions (\c ERS_PRECONDITION) should be used to check condition when
      		entering functions. For instance if you only accept certain values for
                parameters precondtions should verify that those conditions are resepected. 
      <li>Range precondition (\c ERS_RANGE_CHECK) this special type of pre-condition
      	checks that a value is in a range between min and max values. 
      <li>Strict range checking (\c ERS_STRICT_RANGE_CHECK) is similar to the ERS_RANGE_CHECK
      	but does not allow the checked value to be equal to either min or max values. 
      </ul>
  All macros create an object of the ers::Assertion class( which inherits ers::Issue) 
  and send it to the ers::fatal stream. Futher processing depends on the configuration of the
  ers::fatal stream. By default the issue is pronted to the standard error stream and then abort
  function is called.

  \subsection LoggingMacros Logging Macros
  The ERS package offers a set of macros to do logging. Those macros construct an 
  issue and send them to the relevant stream. 
  They all support multiple number of parameters with \c fprintf like patterns.
  For each debug and warning severity_t level there is an associated macro:
  \li ERS_DEBUG - sends ers::Info issue to the ers::debug stream
  \li ERS_INFO - sends ers::Info issue to the ers::information stream
  \li ERS_WARNING - sends ers::Info issue to the ers::warning stream

  The actual behaviour of these macros depends on the configuration of respective streams. 
  Debug macros with levels larger than 0 can be disabled at run-time by defining the
  TDAQ_ERS_DEBUG_LEVEL environment variable to the highest possible debug level.
  For instance, if TDAQ_ERS_DEBUG_LEVEL is set to N, then ERS_DEBUG( M, ... ) where M > N
  will not be processed. The default value for the TDAQ_ERS_DEBUG_LEVEL is 0.

  The amount of information, which is printed for any issue depends on the ERS verbosity level,
  which is set to 0 by default. In this case the follwing infomation is reported for any issue:
  \li severity
  \li time
  \li origin, which includes package name, file name, function name and line number
  \li issue's message
  
  One can increase severity level by using the following command:
  \code > export TDAQ_ERS_VERBOSITY_LEVEL=N \endcode
  where N is any positive integer or zero.
  
  \li For N > 0 th following infomation the issue attributes names and values are reported in addition
  to the 0-level data
  
  \li For N > 1 the following data is added:
  <ul><li> host name
  <li> user name
  <li> process id
  <li> process current working directory
  </ul>
  
  \li For N > 2 the following data is added the stack trace for the current issue is reported.

  \section Issues Building Custom Issues
  ERS assumes that user functions should throw exceptions in case of errors. If such exceptions
  are instances of classes, which inherit the ers::Issue one, ERS offers different advantages in their
  handling:
  \li a possibility to report such exceptions to different streams
  \li a possibility to use chaines of exceptions in order to keep as much as possible infomation
  	about the issue, which has happened.
  \li a unified output operator, which can be used to print exceptions
  
  In order to define a custom issue one has to do the following:
  \li declare a class, which inherits the ers::Issue one
  \li implement 3 pure virtual functions, declared in the ers:Issue class
  \li register new Issue using the ers::IssueFactory::register_issue function
  
  ERS defines helper macro, which does all the 3 steps. The macro is called ERS_DECLARE_ISSUE
  and can be used as shown on the following example: 
  \code
  ERS_DECLARE_ISSUE(	ers,								// namespace name 
			Assertion,							// issue name 
			"Assertion (" << condition << ") failed because " << reason,	// message 
			((const char *)condition )					// first attribute 
			((const char *)reason ) )					// second attribute 
  \endcode

  Note that attribute names may appear in the message expression. Also notice a special
  syntax of the attributes declaration, which must always be declared like \c ((attribute_type)attribute_name).
  All the brackets in this expression are essential. Do not put commas between different attributes.
  The result of the ERS_DECLARE_ISSUE macro expansion will look like:
  \code
    namespace ers {
  	class Assertion : public ers::Issue {
	    template <class> friend class ers::IssueRegistrator;
            Assertion() { ; }
            static const bool registered = ers::IssueRegistrator< ers::Assertion >::instance.done;
            static const char * get_uid() { return "ers::Assertion"; }

            virtual void raise() const throw( std::exception ) { throw *this; }
            virtual const char * get_class_name() const { return get_uid(); }
            virtual ers::Issue * clone() const { return new ers::Assertion( *this ); }
            
          public:
            Assertion( const ers::Context & context , const char * condition , const char * reason )
              : ers::Issue( context ) {
              	set_value( "condition", condition );
                set_value( "reason", reason );
                std::ostringstream out;
                out << "Assertion (" << condition << ") failed because " << reason;
                set_message( out.str() );
            }
            
            Assertion( const ers::Context & context, const std::string & msg , const char * condition , const char * reason )
              : ers::Issue( context, msg ) {
              	set_value( "condition", condition );
                set_value( "reason", reason );
            }
            
            Assertion( const ers::Context & context , const char * condition , const char * reason , const std::exception & cause )
              : ers::Issue( context, cause ) {
              	set_value( "condition", condition );
                set_value( "reason", reason );
                std::ostringstream out;
                out << "Assertion (" << condition << ") failed because " << reason;
                set_message( out.str() );
            }
            
            const char * get_condition () {
            	const char * val;
                get_value( "condition", val );
                return val;
            }
            
            const char * get_reason () {
            	const char * val;
                get_value( "reason", val );
                return val;
            }
	};
    }
  \endcode
  \see ers::Issue 
  \see ExampleIssues.h

  \section Exception handling
  Functions, which can throw exceptions must be invoked insize the \c try...catch statement. The
  following example shows how one can handle ERS exceptions.
  First of all one has to declare once all the possible exceptions:
  \see ExampleIssues.h
 
  
  \code
    #include <ers/ExampleIssues.h>
    
    ...
    
    try
    {
	foo( );
    }
    catch ( ers::PermissionDenied & ex )
    {
	ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	ers::warning( issue );
    }
    catch ( ers::FileDoesNotExist & ex )
    {
	ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	ers::warning( issue );
    }
    catch ( ers::Issue & ex )
    {
	ERS_DEBUG( 0, "Unknown issue caught: " << ex )
	ers::error( ex );
    }
    catch ( std::exception & ex )
    {
	ers::CantOpenFile issue( ERS_HERE, "unknown", ex );
	ers::warning( issue );
    }
  \endcode  
  This example shows the main features of the ERS issues, namely:
  \li an issue does not have severity by itself. Severity of the issue is defined depending on the
  	stream to which this issue is reported.
  \li an issue can be send to one of the existing ERS streams using one of the following functions:
  ers::debug, ers::info, ers::warning, ers::error, ers::fatal
  \li a user defined issue has a constructor, which accept another issue as parameter. The former
  	issue will keep the copy of the later one and can show it on request.
  \li a user defined issue has a constructor, which accept std::exception issue as parameter.
  	The former issue will keep the copy of the later one and can show it on request.
  
  \section Configuring Streams
  The ERS system use the abstraction of streams to handle issues. 
  Conceptualy, a stream is simply an object that can the user can use to send (or receive) Issues. 
  Several streams can be associated with each severity level. If there is more then one stream
  defined for a certain severity then every issue will be passed sequentially to all of those
  stream. In the current ERS version the default configuration of the ERS streams looks like:
  \li ers::debug - "stdout" - prints issues to the standard C++ output stream
  \li ers::information - "stdout" - prints issues to the standard C++ output stream
  \li ers::warning - "stderr" - prints issues to the standard C++ error stream
  \li ers::error - "stderr" - prints issues to the standard C++ error stream
  \li ers::fatal - "stderr,abort" - prints issues to the standard C++ error stream and calls abort function

  In order to change the default configuration for any ERS stream one has to define
  the TDAQ_ERS_<SEVERITY> environment variable. For example the following command:
  \code > export TDAQ_ERS_ERROR="stderr,throw" \endcode
  will cause all the issues, which are sent to the ers::error stream, to be printed to 
  the standard C++ error stream and being thrown using the C++ throw operator.
  A filtering stream can also be associated with any severity level. For example:
  \code > export TDAQ_ERS_ERROR="stderr,filter(ipc),throw" \endcode
  The difference with the previous configuration is that only errors, which have been sent
  by the "ipc" CMT package (for example by the libipc.so library), will be given to the "throw"
  stream. Errors from all the other CMT packages will be only printed to the standard error
  stream. One can also define complex and reversed filters like in the following example:
  \code > export TDAQ_ERS_ERROR="stderr,filter(!ipc,!is),throw" \endcode
  This configuration will throw all the errors, which are neither from "ipc" nor from "is"
  CMT packages.
  

  \section FAQ FAQ  
  \subsection ERS_HERE What is the macro ERS_HERE?
  The macro ERS_HERE is used to insert all the context information to a ers issue. 
  When constructing an issue one should always give the macro ERS_HERE as a first parameter. 
  \see ers::Context

*/
