/*
 *  ers.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.01.05.
 *  Modified by Serguei Kolos on 26.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file ers.h This file defines the basic ERS API.
  * \author Serguei Kolos
  * \version 1.1
  * \brief ers main header and documentation file
  */

#ifndef ERS_ERS_H
#define ERS_ERS_H

#include <sstream>
#include <ers/StreamManager.h>
#include <ers/Configuration.h>
#include <ers/Issue.h>
#include <ers/Assertion.h>
#include <ers/Severity.h>
#include <ers/LocalStream.h>

#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/function.hpp>

/*! \namespace ers
 *  This is a wrapping namespace for all ERS classes and global functions.
 */

namespace ers
{
    typedef Issue Exception;
    
    /*! \namespace thread
     *	This is a wrapping namespace for ERS classes and global functions
     *  which can be used for the local inter-thread error reporting.
     */

    /*!
     *	This function sets up the local issue handler function. This function will be executed in the context
     *	of dedicated thread which will be created as a result of this call. All the issues which are reported
     *	via the ers::error, ers::fatal and ers::warning functions will be forwarded to this thread.
     *	\param issue the issue to be reported
     *	\return pointer to the handler object, which allows to remove the catcher by just destroying this object.
     *			If an applications ignores this return value there will no way of de installing the issue catcher.
     *	\throw ers::IssueCatcherAlreadySet for safety reasons local issue handler can be set only once
     *	\see ers::error()
     *	\see ers::fatal()
     *	\see ers::warning()
     */
    inline IssueCatcherHandler * 
    	set_issue_catcher( const boost::function<void ( const ers::Issue & )> & catcher )
    { return LocalStream::instance().set_issue_catcher( catcher ); }
    
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
    { LocalStream::instance().error( issue ); }
    
    /*! 
     *  This function sends the issue to the ERS FATAL stream.
     *  \param issue the issue to be reported
     */
    inline void fatal( const Issue & issue )
    { LocalStream::instance().fatal( issue ); }
    
    /*! 
     *  This function sends the issue to the ERS INFO stream.
     *  \param issue the issue to be reported
     */
    inline void info( const Issue & issue )
    { StreamManager::instance().information( issue ); }
    
    /*! 
     *  This function sends the issue to the ERS LOG stream.
     *  \param issue the issue to be reported
     */
    inline void log( const Issue & issue )
    { StreamManager::instance().log( issue ); }
    
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
    { LocalStream::instance().warning( issue ); }
}

ERS_DECLARE_ISSUE( ers, Message, ERS_EMPTY, ERS_EMPTY )

#define ERS_REPORT_IMPL( stream, issue, message, level ) \
{ \
    std::ostringstream ers_report_impl_out_buffer; \
    ers_report_impl_out_buffer << message; \
    stream( issue( ERS_HERE, ers_report_impl_out_buffer.str() ) \
	    BOOST_PP_COMMA_IF( BOOST_PP_NOT( ERS_IS_EMPTY( ERS_EMPTY level ) ) ) level ); \
}

#ifndef ERS_NO_DEBUG
/** \def ERS_DEBUG( level, message) This macro sends the message to the ers::debug stream
 * if level is less or equal to the TDAQ_ERS_DEBUG_LEVEL, which is equal to 0 by default.
 * \note This macro is defined to empty statement if the \c ERS_NO_DEBUG macro is defined
 */
#define ERS_DEBUG( level, message ) do { \
if ( ers::debug_level() >= level ) \
{ \
    ERS_REPORT_IMPL( ers::debug, ers::Message, message, level ); \
} } while(0)
#else
#define ERS_DEBUG( level, message ) do { } while(0)
#endif

/** \def ERS_INFO( message ) This macro sends the message to the ers::info stream.
 */
#define ERS_INFO( message ) do { \
{ \
    ERS_REPORT_IMPL( ers::info, ers::Message, message, ERS_EMPTY ); \
} } while(0)

/** \def ERS_LOG( message ) This macro sends the message to the ers::log stream.
 */
#define ERS_LOG( message ) do { \
{ \
    ERS_REPORT_IMPL( ers::log, ers::Message, message, ERS_EMPTY ); \
} } while(0)

#endif // ERS_ERS_H

/** \page main How to use ERS package

  \li \ref macro
      <ul>
      <li> \ref Assertionmacro
      <li> \ref Loggingmacro
      </ul>
  \li \ref CustomIssues
  \li \ref ExceptionHandling
  \li \ref StreamConfig
  \li \ref CustomStream
      <ul>
      <li> \ref ImplementingCustomStream
      <li> \ref RegisteringCustomStream
      <li> \ref UsingCustomStream
      </ul>
  \li \ref MultiThreadError
      <ul>
      <li> \ref MultiThreadReporting
      <li> \ref ErrorCatcherThread
      </ul>
  \li \ref ERS_HERE


  The Error Reporting System (ERS) software package provides a common API for error reporting
  in the ATLAS TDAQ system.
  ERS offers several macro that can be used for reporting pre-defined errors if some
  conditions are violated at the level of C++ code. ERS also provides tools for defining
  custom classes that can be used for reporting high-level issues.

  \section Header Header file
  In order to use all available ERS functionality an application has to include a single
  header file \c ers/ers.h.
  
  \section AssertionMacro Assertion Macro

  ERS provides several convenience macro for checking conditions in a C++ code. If a certain condition
  is violated a corresponding macro creates a new instance of \c ers::Assertion class and sends it to
  the \c ers::fatal stream. Further processing depends on the \c ers::fatal stream configuration.
  By default the issue is printed to the standard error stream.

  Here is a list of available macro:
  \li ERS_ASSERT( expression ) generic macro that checks whether a given expression is valid.
  \li ERS_PRECONDITION( expression ) the same as ERS_ASSERT but uses a message that is adopted
      for reporting an invalid input parameter for a function.
  \li ERS_RANGE_CHECK( min, val, max ) is a special type of pre-condition, which checks that a value
  is in a range between min and max values. 
  \li ERS_STRICT_RANGE_CHECK( min, val, max ) is similar to the ERS_RANGE_CHECK
  but does not allow the checked value to be equal to either min or max values. 

  \note All These macro are defined to an empty statement if the \c ERS_NO_DEBUG macro is defined
  at compilation time.

  \section LoggingMacro Logging Macro
  The ERS package offers a set of macro to do logging:
  \li ERS_DEBUG( level, message ) - sends ers::Message issue to the ers::debug stream
  \li ERS_LOG( message ) - sends ers::Message issue to the ers::log stream
  \li ERS_INFO( message ) - sends ers::Message issue to the ers::information stream
  \note ERS_DEBUG macro is defined to empty statement if the \c ERS_NO_DEBUG macro is defined at compilation time.
  
  Each of these macro constructs an new issue of ers::Message time and sends it to an appropriate stream.
  The \b message argument of these macro can be any entity, for which the standard C++ output stream operator
  (\c operator<<) is defined. This means that the message can be a single value of a certain type as well
  as a sequence of output operations of appropriate types.
  For example:
  \code ERS_DEBUG( 1, "simple debug output " << 123 << " that shows how to use debug macro" )
  \endcode
  
  The actual behavior of these macro depends on the configuration of a respective streams.
  Debug macro can be disabled at run-time by defining the
  TDAQ_ERS_DEBUG_LEVEL environment variable to the highest possible debug level.
  For instance, if TDAQ_ERS_DEBUG_LEVEL is set to N, then ERS_DEBUG( M, ... ) where M > N
  will not be processed. The default value for the TDAQ_ERS_DEBUG_LEVEL is 0. Negative
  debug levels are also supported.

  The amount of information, which is printed for an issue depends on the actual ERS verbosity level,
  which can be controlled via the TDAQ_ERS_VERBOSITY_LEVEL macro. Default verbosity value is zero.
  In this case the following information is reported for any issue:
  \li severity (DEBUG, LOG, INFO, WARNING, ERROR, FATAL)
  \li the time of the issue occurrence
  \li the issue's context, which includes package name, file name, function name and line number
  \li the issue's message
  
  One can control the current severity level via the TDAQ_ERS_VERBOSITY_LEVEL macro:
  \code > export TDAQ_ERS_VERBOSITY_LEVEL=N \endcode
  where N is any integer number.
  
  \li For N > 0 the issue attributes names and values are reported in addition to 0-level data
  
  \li For N > 1 the following information is added to the issue:
  <ul>
  <li> host name
  <li> user name
  <li> process id
  <li> process current working directory
  </ul>
  
  \li For N > 2 a stack trace is added to each issue if the actual code was compiled without ERS_NO_DEBUG macro.

  \section CustomIssues Building Custom Issues
  ERS assumes that user functions should throw exceptions in case of errors. If such exceptions
  are instances of classes, which inherit the ers::Issue one, ERS offers different advantages in their
  handling:
  \li a possibility to report such exceptions to different streams
  \li a possibility to use chains of exceptions in order to keep as much as possible information
  	about the issue, which has happened.
  \li a unified output operator, which can be used to print exceptions
  
  In order to define a custom issue one has to do the following:
  \li declare a class, which inherits the ers::Issue
  \li implement 3 pure virtual functions, declared in the ers:Issue class
  \li register new Issue using the ers::IssueFactory::register_issue function
  
  ERS defines two helper macro, which implement all these steps. The macro are called ERS_DECLARE_ISSUE
  and ERS_DECLARE_ISSUE_BASE. The first one should be used to declare the issue class, which inherits
  from the ers::Issue as it is shown on the following example: 
  \code
  ERS_DECLARE_ISSUE(
      ers,					                                        // namespace
	  Assertion,				                                    // issue name
	  "Assertion (" << condition << ") failed because " << reason,	// message
	  ((const char *)condition )		                            // first attribute
	  ((const char *)reason )			                            // second attribute
  )
  \endcode

  Note that attribute names may appear in the message expression. Also notice a special
  syntax of the attributes declaration, which must always be declared like \c ((attribute_type)attribute_name).
  All the brackets in this expression are essential. Do not use commas to separate attributes.
  The only requirement to the type of an issue attribute is that such a type must define the output
  operator to the standard C++ output stream and the input operator from the standard C++ input
  stream. It is important to note that these operators must  unambiguously match each other, i.e.
  the input operator must be able to unambiguously restore the state of the attribute from a stream,
  which had been used to save the object's state with the output operator. Evidently all the
  standard C++ types satisfy this criteria.
  A result of the ERS_DECLARE_ISSUE macro expansion looks like:
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
  \see SampleIssues.h

  The macro ERS_DECLARE_ISSUE_BASE has to be used in case one wants to declare a new issue class,
  which inherits from one of the issue classes declared with either this or ERS_DECLARE_ISSUE
  macro. For example, the following class inherits from the ers::Assertion class defined above:
  \code
  ERS_DECLARE_ISSUE_BASE(ers,							        // namespace name
		Precondition,						                    // issue name
		ers::Assertion,						                    // base issue name
		"Precondition (" << condition << ") located in " << location
                        	<< " failed because " << reason,	// message
		((const char *)condition ) ((const char *)reason ),	    // base class attributes
		((const char *)location )				                // this class attributes
  )
  \endcode
  
  A result of the ERS_DECLARE_ISSUE_BASE macro expansion looks like:
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

  \section ERS_HERE ERS_HERE macro
  The macro ERS_HERE is a convenience macro that is used to insert the context information, like file name,
  line number and function where the issue was constructed, to the new issue. Therefore when a new issue is
  created one shall always use ERS_HERE macro as the first parameter of the issue constructor.

  \section ExceptionHandling Exception handling
  Functions, which can throw exceptions must be invoked inside \c try...catch statement.
  The following example shows a typical use case for ERS exceptions.
  
  \code
    #include <ers/SampleIssues.h>
    
    ...
    
    try {
	    foo( );
    }
    catch ( ers::PermissionDenied & ex ) {
	    ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	    ers::warning( issue );
    }
    catch ( ers::FileDoesNotExist & ex ) {
	    ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	    ers::warning( issue );
    }
    catch ( ers::Issue & ex ) {
	    ERS_DEBUG( 0, "Unknown issue caught: " << ex )
	    ers::error( ex );
    }
    catch ( std::exception & ex ) {
	    ers::CantOpenFile issue( ERS_HERE, "unknown", ex );
	    ers::warning( issue );
    }
  \endcode  
  This example shows the main features of the ERS issues, namely:
  \li An issue does not have severity by itself. Severity of the issue is defined by the
  	stream to which this issue is reported.
  \li An issue can be send to one of the existing ERS streams using one of the following functions:
  ers::debug, ers::info, ers::warning, ers::error, ers::fatal
  \li Any ERS issue has a constructor, which accept another issue as last parameter. The new
  	issue will keep the copy of the original one and will report it as its cause.
  \li Any ERS issue has a constructor, which accept std::exception issue as last parameter.
  	The new issue will keep the copy of the given std::exception one and will report it as its cause.
  
  \section StreamConfig Configuring Streams
  The ERS system provides multiple instances of the stream API, one per severity level, to report issues.
  The issues which are sent to different streams may be sent to a different destination depending on a
  particular stream configuration. By default the ERS streams are configured in the following way:
  \li ers::debug - "lstdout" - prints issues to the standard C++ output stream
  \li ers::log - "lstdout" - prints issues to the standard C++ output stream
  \li ers::info - "lstdout" - prints issues to the standard C++ output stream
  \li ers::warning - "lstderr" - prints issues to the standard C++ error stream
  \li ers::error - "lstderr" - prints issues to the standard C++ error stream
  \li ers::fatal - "lstderr" - prints issues to the standard C++ error stream

  \note the letter "l" at the beginning of "lstdout" and "lstderr" names indicates that these stream
  implementations are thread-safe and can be safely used in multi-threaded applications so that
  issues reported from different threads will not be mixed in the output.

  In order to change the default configuration for any ERS stream one can use
  the TDAQ_ERS_<SEVERITY> environment variable. For example the following command:
  \code > export TDAQ_ERS_ERROR="stderr,throw" \endcode
  will cause all the issues, which are sent to the ers::error stream, been printed to 
  the standard C++ error stream and then been thrown using the C++ throw operator.

  A filter stream can also be associated with any severity level. For example:
  \code > export TDAQ_ERS_ERROR="stderr,filter(ipc),throw" \endcode
  The difference with the previous configuration is that only errors, which have "ipc" qualifier
  will be passed to the "throw" stream. Users can add any qualifiers to their specific issues
  by using the \c Issue::add_qualifier function. By default every issue has one qualifier associated
  with it - the name of the TDAQ software package, which builds the binary (a library or an application)
  where the issue was created.

  One can also define complex and reversed filters like in the following example:
  \code > export TDAQ_ERS_ERROR="stderr,filter(!ipc,!is),throw" \endcode
  This configuration will throw all the errors, which are neither from "ipc" nor from "is"
  CMT packages.
  
  \subsection DefaultStreams Existing Stream Implementations
  ERS provides several stream implementations which can be used in any combination in ERS streams configurations.
  Here is the list of available stream implementations:
  \li "stdout" - prints issues to the standard C++ output stream. It is not thread-safe.
  \li "stderr" - prints issues to the standard C++ error stream. It is not thread-safe.
  \li "lstdout" - prints issues to the standard C++ output stream. It is thread-safe.
  \li "lstderr" - prints issues to the standard C++ error stream. It is thread-safe.
  \li "lock" - locks a global mutex for the duration of reporting an issue to the next streams in the given configuration.
      This stream can be used for adding thread-safety to an arbitrary non-thread-safe stream implementation. For example
          "lock,stdout" configuration is equivalent to "lstdout".
  \li "null" - silently drop any reported issue.
  \li "throw" - apply the C++ throw operator to a reported issue
  \li "abort" - calls abort() function for any issue reported
  \li "exit" - calls exit() function for any issue reported
  \li "filter(A,B,!C,...)" - pass issues which have either A or B and don't have C qualifier.
  \li "filter(RA,RB,!RC,...)" - the same as "filter" stream but treats all the given parameters as regular expressions.
  \li "throttle(initial_threshold, time_interval)" - rejects the same issues reported within the \c time_interval after
      passing through the \c initial_threshold number of them.

  \section CustomStream Custom Stream Implementation
  While ERS provides a set of basic stream implementations one can also implement a custom one if this is required.
  Custom streams can be plugged into any existing application which is using ERS without rebuilding this application.
  
  \subsection ImplementingCustomStream Implementing a Custom Stream
  In order to provide a new custom stream implementation one has to declare a sub-class of the ers::OutputStream
  class and implement the pure virtual method 'write' which is declared in this class.
  Here is an example of how this is done by the standard FilterStream stream implementation:
  \code
  void
  ers::FilterStream::write( const ers::Issue & issue )
  {
	if ( is_accepted( issue ) ) {
	  chained().write( issue );
	}
  }
  \endcode
  The implementation of the ers::OutputStream::write function must decide whether to pass the given 
  issue to the next stream in the chain or not. If a custom stream does not provide any filtering
  functionality then it shall always pass the input message to the next stream by calling the
  \c chained().write( issue ); function.

  \subsection RegisteringCustomStream Registering a Custom Stream
  In order to register and use a custom ERS stream implementation one can use a simple macro called
  ERS_REGISTER_STREAM in the followiung way:
  \code
  ERS_REGISTER_OUTPUT_STREAM( ers::FilterStream, "filter", format )
  \endcode

  The first parameter of this macro is the name of the class which implements the stream; the second one
  gives a new stream name to be used in ERS stream configurations (this is the name which one can put to the
  TDAQ_ERS_<SEVERITY> environment variables); and the last parameter is a placeholder for the stream class
  constructor parameters. If the constructor of the new custom stream does not require parameters then last
  field of this macro should be left empty.

  \subsection UsingCustomStream Using Custom Stream
  In order to use a custom stream one has to build a new shared library from the class that implements this stream
  and then pass this library to ERS by setting its name to the TDAQ_ERS_STREAM_LIBS environment variable.
  For example if this macro is set to the following value:
  \code
  export  TDAQ_ERS_STREAM_LIBS=MyCustomFilter
  \endcode
  then ERS will be looking for the libMyCustomFilter.so library in all the directories which appear in the 
  LD_LIBRARY_PATH environment variable.

  \section MultiThreadError Error Reporting in Multi-threaded Applications
  ERS can be used for error reporting in multi-threaded applications. As C++ language does not provide a way of
  passing exceptions across thread boundaries, ERS provides the \c ers::set_issue_catcher function to overcome this
  limitation.
  When one of the threads of a software application catches an issue it can send it to one of the the ERS
  streams using \c ers::error, \c ers::fatal or \c ers::warning functions. If no error catcher thread was installed
  in this application the new issue will be forwarded to the respective ERS stream implementation according to the
  actual stream configuration. If on the other hand a custom issue catcher was installed the issue will be passed
  to the dedicated thread which will call the custom error catcher function.

  \subsection ErrorCatcherThread Setting up an Error Catcher
  An error catcher should be installed by calling the \c ers::set_issue_catcher function and passing
  it a function object as parameter. This function object will be executed in the context of a dedicated
  thread (created by the \c ers::set_issue_catcher function) for every issue which is reported by the current
  application to ERS fatal, error and warning streams.
  The parameter of the ers::set_issue_catcher is of \c boost::function<void ( const ers::Issue & )> type
  which allows to use plain C-style functions as well as C++ member functions for implementing a custom error
  catcher. For example one can define an error catcher as a member function of a certain class:
  \code
  struct IssueCatcher
  {
    void handler( const ers::Issue & issue ) {
	    std::cout << "IssueCatcher has been called: " << issue << std::endl;
    }
  };
  \endcode

  A new error catcher has to be registered with ERS in the following way:
  \code
  IssueCatcher * catcher = new IssueCatcher();
  try {
    ers::set_issue_catcher( boost::bind( &IssueCatcher::handler, catcher, _1 ) );
  }
  catch( ers::IssueCatcherAlreadySet & ex ){
    ...
  }
  \endcode
  Note that the error handling catcher can be set only once for the lifetime of an application. 
  An attempt to set it again will fail and the \c ers::IssueCatcherAlreadySet exception will be thrown.
  
  \section Receiving Issues Across Application Boundaries
  There is a specific implementation of ERS input (and output) stream which allows to exchange issue
  across application boundaries, i.e. one process may receive ERS issues produces by the other processes.
  The following example shows how to do that:
  
  \code
    #include <ers/InputStream.h>
    #include <ers/ers.h>
    
    struct MyIssueReceiver : public ers::IssueReceiver
    {
	    void receive( const ers::Issue & issue ) {
	        std::cout << issue << std::endl;
	    }
    };

    MyIssueReceiver * receiver = new MyIssueReceiver;
    try {
	    ers::StreamManager::instance().add_receiver( "mts", "*", receiver );
    }
    catch( ers::Issue & ex ) {
	    ers::fatal( ex );
    }
  \endcode
  
  The MyIssueReceiver instance will be getting all messages, which are sent to the "mts" stream implementation
  by all applications working in the current TDAQ partition, which will be taken from the TDAQ_PARTITION environment variable.
  Alternatively one may pass partition name explicitely via the "mts" stream parameters list:
  
  \code
    std::string partition_name = ... // initialize it to a desired name
    MyIssueReceiver * receiver = new MyIssueReceiver;
    try {
	    ers::StreamManager::instance().add_receiver( "mts", {partition_name, "*"}, receiver );
    }
    catch( ers::Issue & ex ) {
	    ers::fatal( ex );
    }
  \endcode
  
  One can also cancel a previously made subscription be calling the \c ers::StreamManager::remove_receiver
  function and giving it a pointer to the corresponding receiver object, e.g.:
  
  \code
    try {
	    ers::StreamManager::instance().remove_receiver( receiver );
    }
    catch( ers::Issue & ex ) {
	    ers::fatal( ex );
    }
  \endcode
*/
