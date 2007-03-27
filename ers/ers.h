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
#include <ers/StreamManager.h>
#include <ers/Configuration.h>
#include <ers/Issue.h>
#include <ers/Assertion.h>
#include <ers/Severity.h>

#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

/*! \namespace ers
 *  This is a wrapping namespace for all ERS classes and global functions.
 */

namespace ers
{
    typedef Issue Exception;
    
    /*! 
     *  This function returns the current debug level for ERS.
     */
    inline int debug_level( )
    { return Configuration::instance().debug_level( ); }
    
    /*! 
     *  This function sends the issue to the ERS DEBUG stream which corresponds to the given debug level.
     *  \param issue the issue to be reported
     *  \level debug level which will be associated with the reported issue
     */
    inline void debug( const Issue & issue, int level = debug_level() )
    { StreamManager::instance().debug( issue, level ); }
    
    /*! 
     *  This function sends the issue to the ERS ERROR stream.
     *  \param issue the issue to be reported
     */
    inline void error( const Issue & issue )
    { StreamManager::instance().error( issue ); }
    
    /*! 
     *  This function sends the issue to the ERS FATAL stream.
     *  \param issue the issue to be reported
     */
    inline void fatal( const Issue & issue )
    { StreamManager::instance().fatal( issue ); }
    
    /*! 
     *  This function sends the issue to the ERS INFO stream.
     *  \param issue the issue to be reported
     */
    inline void info( const Issue & issue )
    { StreamManager::instance().information( issue ); }
    
    /*! 
     *  This function returns the current verbosity level for ERS.
     */
    inline int verbosity_level( )
    { return Configuration::instance().verbosity_level( ); }

    /*! 
     *  This function sends the issue to the ERS WARNING stream.
     *  \param issue the issue to be reported
     */
    inline void warning( const Issue & issue )
    { StreamManager::instance().warning( issue ); }
}

ERS_DECLARE_ISSUE( ers, Message, , )

#define ERS_REPORT_IMPL( stream, issue, message, level ) \
{ \
    std::ostringstream ers_report_impl_out_buffer; \
    ers_report_impl_out_buffer << message; \
    stream( issue( ERS_HERE, ers_report_impl_out_buffer.str() ) \
	    BOOST_PP_COMMA_IF( BOOST_PP_NOT( BOOST_PP_IS_EMPTY( level ) ) ) level ); \
}

#ifndef ERS_NO_DEBUG
/** \def ERS_DEBUG( level, message) This macro sends the message to the ers::debug stream
 * if level is less or equal to the TDAQ_ERS_DEBUG_LEVEL, which is equal to 0 by default.
 * \note This macro is defined to empty statement if the \c ERS_NO_DEBUG macro is defined
 */
#define ERS_DEBUG( level, message ) { \
if ( ers::debug_level() >= level ) \
{ \
    ERS_REPORT_IMPL( ers::debug, ers::Message, message, level ); \
} }
#else
#define ERS_DEBUG( level, message ) { }
#endif

/** \def ERS_INFO( message ) This macro sends the message to the ers::info stream.
 */
#define ERS_INFO( message ) \
{ \
    ERS_REPORT_IMPL( ers::info, ers::Message, message, ); \
}
    	
#endif

/** \page main How to use the ERS package
  The goal of the Error Reporting System is to offer tool to simplify and unify error 
  handling and error reporting in TDAQ applications. 
  This package can be used at different levels. At the lowest level, one can simply 
  use the existing checking and logging macros. 
  For error reporting one has to define specific Issue subclasses. 
  \section Macros Basic macros
  Basic ERS functionality can be exploited by using simple macros. 
  Those macros are available simply by including the ers/ers.h header file. 
  \subsection AssertionMacros Assertion Macros
  The ERS package offers a set of macros to do basic checking. The behavior of these macros
  is defined by the implementation of the ers::fatal stream, which can be configured by the user:
  \li ERS_ASSERT( expression ) checks whether a given expression is valid. 
  \li ERS_PRECONDITION( expression ) should be used to check condition when entering functions.
  For instance if you only accept certain values for parameters precondtions should verify that
  those conditions are resepected. 
  \li ERS_RANGE_CHECK( min, val, max ) is a special type of pre-condition, which checks that a value
  is in a range between min and max values. 
  \li ERS_STRICT_RANGE_CHECK( min, val, max ) is similar to the ERS_RANGE_CHECK
  but does not allow the checked value to be equal to either min or max values. 

  All macros create an object of the ers::Assertion class( which inherits ers::Issue) 
  and send it to the ers::fatal stream. Futher processing depends on the configuration of the
  ers::fatal stream. By default the issue is pronted to the standard error stream and then abort
  function is called.
  \note All These macro are defined to empty statement if the \c ERS_NO_DEBUG macro is defined at compilation time.

  \subsection LoggingMacros Logging Macros
  The ERS package offers a set of macros to do logging. Those macros construct an 
  issue and send them to the relevant stream. 
  For each debug and warning severity_t level there is an associated macro:
  \li ERS_DEBUG( level, message ) - sends ers::Info issue to the ers::debug stream
  \li ERS_INFO( message ) - sends ers::Info issue to the ers::information stream
  \li ERS_WARNING( message ) - sends ers::Info issue to the ers::warning stream
  \note ERS_DEBUG macro is defined to empty statement if the \c ERS_NO_DEBUG macro is defined at compilation time.
  
  The \b message argument of these macros can be any entity, for which the operator<< to the
  standard C++ output stream is defined. This means that the message can be a single 
  value of a certain type as well as a sequence of output operations of appropriate types.
  For example:
  \code ERS_DEBUG( 1, "test debug output " << 123 << " which shows how to use debug macro" )
  \endcode
  
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
  
  ERS defines two helper macros, which do all the 3 steps. The macros are called ERS_DECLARE_ISSUE
  and ERS_DECLARE_ISSUE_BASE. The first one should be used to declare the issue class, which inherits
  from the ers::Issue as it is shown on the following example: 
  \code
  ERS_DECLARE_ISSUE(	ers,					// namespace name 
			Assertion,				// issue name 
			"Assertion (" << condition 
                        << ") failed because " << reason,	// message 
			((const char *)condition )		// first attribute 
			((const char *)reason )			// second attribute 
  		   )
  \endcode

  Note that attribute names may appear in the message expression. Also notice a special
  syntax of the attributes declaration, which must always be declared like \c ((attribute_type)attribute_name).
  All the brackets in this expression are essential. Do not put commas between different attributes.
  The only requiremnt to the type of issue attributes is that such type must define the output 
  operator to the standard C++ output stream and the input operator from the standard C++ input
  stream. It is important to note that these operator must  unambiguously match each other, i.e.
  the input operator must be able to unambiguously restore the state of the object from the stream,
  which has been used to save the object's state using the output operator. Evidently all the
  standard C++ types satisfy this criteria.
  The result of the ERS_DECLARE_ISSUE macro expansion will look like:
  \code
    namespace ers {
  	class Assertion : public ers::Issue {
	    template <class> friend class ers::IssueRegistrator;
            Assertion() { ; }
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

  The macro ERS_DECLARE_ISSUE_BASE has to be used in case one wants to declare the issue class,
  which inherits from one of the issue classes declared with either this or ERS_DECLARE_ISSUE
  macro. For example, the following class inherits from the ers::Assertion class defined above:
  \code
  ERS_DECLARE_ISSUE_BASE(	ers,							// namespace name 
				Precondition,						// issue name 
				ers::Assertion,						// base issue name 
				"Precondition (" << condition 
                        	<< ") located in " << location 
                        	<< " failed because " << reason,			// message 
				((const char *)condition ) ((const char *)reason ),	// base class attributes
				((const char *)location )				// this class attributes
			)
  \endcode
  
  The result of the ERS_DECLARE_ISSUE_BASE macro expansion will look like:
  \code
    namespace ers {
  	class Precondition : public ers::Assertion {
	    template <class> friend class ers::IssueRegistrator;
            Precondition() { ; }
            static const bool registered = ers::IssueRegistrator< ers::Precondition >::instance.done;
            static const char * get_uid() { return "ers::Precondition"; }

            virtual void raise() const throw( std::exception ) { throw *this; }
            virtual const char * get_class_name() const { return get_uid(); }
            virtual ers::Issue * clone() const { return new ers::Precondition( *this ); }
            
          public:
            Precondition( const ers::Context & context , const char * condition , const char * reason, const char * location )
              : ers::Assertion( context, condition, reason ) {
              	set_value( "location", location );
                std::ostringstream out;
                out << "Precondition (" << condition << ") located in " << location << ") failed because " << reason;
                set_message( out.str() );
            }
            
            Precondition( const ers::Context & context, const std::string & msg , const char * condition , const char * reason, const char * location )
              : ers::Assertion( context, msg, condition, reason ) {
              	set_value( "location", location );
            }
            
            Precondition( const ers::Context & context , const char * condition , const char * reason , const char * location, const std::exception & cause )
              : ers::Assertion( context, condition, reason, cause ) {
              	set_value( "location", location );
                std::ostringstream out;
                out << "Precondition (" << condition << ") located in " << location << ") failed because " << reason;
                set_message( out.str() );
            }
            
            const char * get_location () {
            	const char * val;
                get_value( "location", val );
                return val;
            }
	};
    }
  \endcode

  \section Exception Exception handling
  Functions, which can throw exceptions must be invoked inside the \c try...catch statement. The
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
  \li ers::debug - "lstdout" - prints issues to the standard C++ output stream
  \li ers::info - "lstdout" - prints issues to the standard C++ output stream
  \li ers::warning - "lstderr" - prints issues to the standard C++ error stream
  \li ers::error - "lstderr" - prints issues to the standard C++ error stream
  \li ers::fatal - "lstderr" - prints issues to the standard C++ error stream

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
  

  \section ERS_HERE ERS_HERE macro
  The macro ERS_HERE is used to insert all the context information to a ers issue. 
  When constructing an issue one should always give the macro ERS_HERE as a first parameter. 
  \see ers::Context

*/
