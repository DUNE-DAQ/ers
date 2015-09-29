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
  * \author Serguei Kolos
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
	    throw ( ers::IssueCatcherAlreadySet )
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

/** \page main How to use the ERS package

  \li \ref Macros
      <ul>
      <li> \ref AssertionMacros
      <li> \ref LoggingMacros
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
  For instance if you only accept certain values for parameters pre-conditions should verify that
  those conditions are respected.
  \li ERS_RANGE_CHECK( min, val, max ) is a special type of pre-condition, which checks that a value
  is in a range between min and max values. 
  \li ERS_STRICT_RANGE_CHECK( min, val, max ) is similar to the ERS_RANGE_CHECK
  but does not allow the checked value to be equal to either min or max values. 

  All macros create an object of the ers::Assertion class( which inherits ers::Issue) 
  and send it to the ers::fatal stream. Further processing depends on the configuration of the
  ers::fatal stream. By default the issue is printed to the standard error stream and then abort
  function is called.
  \note All These macro are defined to empty statement if the \c ERS_NO_DEBUG macro is defined at compilation time.

  \subsection LoggingMacros Logging Macros
  The ERS package offers a set of macros to do logging. Those macros construct an 
  issue and send them to the relevant stream. 
  For each debug and warning severity_t level there is an associated macro:
  \li ERS_DEBUG( level, message ) - sends ers::Message issue to the ers::debug stream
  \li ERS_INFO( message ) - sends ers::Message issue to the ers::information stream
  \li ERS_LOG( message ) - sends ers::Message issue to the ers::log stream
  \note ERS_DEBUG macro is defined to empty statement if the \c ERS_NO_DEBUG macro is defined at compilation time.
  
  The \b message argument of these macros can be any entity, for which the operator<< to the
  standard C++ output stream is defined. This means that the message can be a single 
  value of a certain type as well as a sequence of output operations of appropriate types.
  For example:
  \code ERS_DEBUG( 1, "test debug output " << 123 << " which shows how to use debug macro" )
  \endcode
  
  The actual behavior of these macros depends on the configuration of respective streams.
  Debug macros with levels larger than 0 can be disabled at run-time by defining the
  TDAQ_ERS_DEBUG_LEVEL environment variable to the highest possible debug level.
  For instance, if TDAQ_ERS_DEBUG_LEVEL is set to N, then ERS_DEBUG( M, ... ) where M > N
  will not be processed. The default value for the TDAQ_ERS_DEBUG_LEVEL is 0.

  The amount of information, which is printed for any issue depends on the ERS verbosity level,
  which is set to 0 by default. In this case the following information is reported for any issue:
  \li severity
  \li time
  \li context, which includes package name, file name, function name, line number etc.
  \li issue's message
  
  One can increase severity level by using the following command:
  \code > export TDAQ_ERS_VERBOSITY_LEVEL=N \endcode
  where N is any positive integer or zero.
  
  \li For N > 0 th following information the issue attributes names and values are reported in addition
  to the 0-level data
  
  \li For N > 1 the following data is added:
  <ul><li> host name
  <li> user name
  <li> process id
  <li> process current working directory
  </ul>
  
  \li For N > 2 the following data is added the stack trace for the current issue is reported.

  \section CustomIssues Building Custom Issues
  ERS assumes that user functions should throw exceptions in case of errors. If such exceptions
  are instances of classes, which inherit the ers::Issue one, ERS offers different advantages in their
  handling:
  \li a possibility to report such exceptions to different streams
  \li a possibility to use chains of exceptions in order to keep as much as possible information
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
  The only requirement to the type of issue attributes is that such type must define the output
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
  \see SampleIssues.h

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

  \section ExceptionHandling Exception handling
  Functions, which can throw exceptions must be invoked inside the \c try...catch statement. The
  following example shows how one can handle ERS exceptions.
  First of all one has to declare once all the possible exceptions:
  \see SampleIssues.h
 
  
  \code
    #include <ers/SampleIssues.h>
    
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
  
  \section StreamConfig Configuring Streams
  The ERS system use the abstraction of streams to handle issues. 
  Conceptually, a stream is simply an object that can the user can use to send (or receive) Issues.
  Several streams can be associated with each severity level. If there is more then one stream
  defined for a certain severity then every issue will be passed sequentially to all of those
  streams. In the current ERS version the default configuration of the ERS streams looks like:
  \li ers::debug - "lstdout" - prints issues to the standard C++ output stream
  \li ers::info - "lstdout" - prints issues to the standard C++ output stream
  \li ers::warning - "lstderr" - prints issues to the standard C++ error stream
  \li ers::error - "lstderr" - prints issues to the standard C++ error stream
  \li ers::fatal - "lstderr" - prints issues to the standard C++ error stream

  In order to change the default configuration for any ERS stream one has to define
  the TDAQ_ERS_<SEVERITY> environment variable. For example the following command:
  \code > export TDAQ_ERS_ERROR="stderr,throw" \endcode
  will cause all the issues, which are sent to the ers::error stream, been printed to 
  the standard C++ error stream and then been thrown using the C++ throw operator.
  A filtering stream can also be associated with any severity level. For example:
  \code > export TDAQ_ERS_ERROR="stderr,filter(ipc),throw" \endcode
  The difference with the previous configuration is that only errors, which have "ipc" qualifier
  will be passed to the "throw" stream. Users can add any qualifiers to their specific issues
  by using the Issue::add_qualifier function. By default every issue has one quilifier associated
  with it - the name of the CMT package in which this issue was created. One can also define complex 
  and reversed filters like in the following example:
  \code > export TDAQ_ERS_ERROR="stderr,filter(!ipc,!is),throw" \endcode
  This configuration will throw all the errors, which are neither from "ipc" nor from "is"
  CMT packages.
  
  \section CustomStream Custom Streams
  While ERS provides a set of basic streams one can also implement a custom one if it is required. 
  Custom streams can be plugged into any existing application which is using ERS without rebuilding this application.
  
  \subsection ImplementingCustomStream Implementing custom stream
  In order to do that one has to declare a sub-class of the ers::OutputStream class and implement the 
  pure virtual method 'write' which is declared in this class. For example:
  \code
  void
  ers::FilterStream::write( const ers::Issue & issue )
  {
	if ( is_accepted( issue ) )
	{
	  chained().write( issue );
	}
  }
  \endcode
  The implementation of the ers::OutputStream::write function must decide whether to pass the given 
  issue to the other streams in the chain or not. For example if the custom stream provides some filtering 
  functionality then it can pass the message to the following stream by calling the \c chained().write( issue ); 
  function and can simply do a return if the given issue has to be filtered out.

  \subsection RegisteringCustomStream Registering custom stream
  In order to register custom ERS stream one can use a simple macro called ERS_REGISTER_STREAM. For example
  \code
  ERS_REGISTER_OUTPUT_STREAM( ers::FilterStream, "filter", format )
  \endcode
  Here the first parameter is the name of the class which implements the stream, the second one is the name 
  of the stream which can be used for configuring ERS output (this is the name which one can put to the 
  TDAQ_ERS_<SEVERITY> environment variables) and the last one is a place holder for the parameter of the
  stream class constructor. If the constructor of your custom stream does not require parameters then you 
  should leave the last field empty.

  \subsection UsingCustomStream Using custom stream
  In order to use a custom stream one has to build a library out of the class which implements this stream 
  and then tell to ERS the name of that library by setting it to the TDAQ_ERS_STREAM_LIBS environment variable. 
  For example if one sets this variable to the following value:
  \code
  export  TDAQ_ERS_STREAM_LIBS=MyCustomFilter
  \endcode
  then ERS will be looking for the libMyCustomFilter.so library in all the directories which appear in the 
  LD_LIBRARY_PATH environment variable.

  \section MultiThreadError Error reporting in multi-threaded applications
  ERS can be used for error handling in multi-threaded applications. While ERS requires that all problems within 
  the context of a single application have to be reported via exceptions, C++ language does not provide a way of 
  passing exceptions across thread boundaries. To address this problem ERS provides the set_issue_catcher function
  which is defined in the ers namespace. 
  The following sections explain how to use it.
  
  \subsection MultiThreadReporting Reporting errors to local stream
  While one of the worker threads of an application encounters an issue it can send it to one of the the ERS
  streams using ers::error, ers::fatal or ers::warning functions. If no error catcher thread was set up then 
  such issues will be forwarded to the respective global ERS streams. Otherwise they will be passed to the error
  handling thread.

  \subsection ErrorCatcherThread Setting up error handling threaded
  In order to handle errors which have been reported by a thread of a particular application this application 
  has to set up the error handling thread by calling the ers::set_issue_catcher  function and passing 
  to it a function object as parameter. This function object will be executed in the context of a dedicated 
  thread (created by the  ers::set_issue_catcher) for every issue which is reported to the ERS error, fata or warning streams. 
  The parameter of the  ers::set_issue_catcher is of \c boost::function<void ( const ers::Issue & )> type 
  which allows to use plain C-style functions as well as C++ member functions for implementing the error handling 
  algorithm. For example one can define the error handling algorithms as a member function of a certain class:
  \code
  struct IssueCatcher
  {
    void handler( const ers::Issue & issue )
    {
	std::cout << "IssueCatcher has been called for the following issue:" << std::endl;
	std::cout << "\t" << issue << std::endl;
    }
  };
  \endcode
  and then register it with ERS in the following way:
  \code
  IssueCatcher catcher;
  try {
    ers::set_issue_catcher( boost::bind( &IssueCatcher::handler, &catcher, _1 ) );
  }
  catch( ers::IssueCatcherAlreadySet & ex ){
  ...
  }
  \endcode
  Note that the error handling catcher can be set only once for the lifetime of an application. 
  An attempt to set it again will fail and the ers::IssueCatcherAlreadySet will be thrown.
  
  \section Receiving issues across application boundaries
  There is an implementation of ERS input and output streams which allows to subscribe and receive messages
  across application boundaries, i.e. one process may get ERS messages produces by others. ERS provides API
  for setting up and removing a subscription. This API requires mentioning the name of the cross-boundaries
  streams implementation, which is called "mts". A code to set up ERS subscription may look like:
  
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
  
  This application will be getting all messages which are configured to be sent to the "mts" stream
  from all applications working in the TDAQ partition which is given via the TDAQ_PARTITION environment
  variable. Alternatively one may pass partition name via the "mts" stream parameters list:
  
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
  
  One can also cancel a previously made subscription be calling the remove_receiver function and giving it a pointer to 
  the receiver object, e.g.:
  
  \code
    try {
	ers::StreamManager::instance().remove_receiver( receiver );
    }
    catch( ers::Issue & ex ) {
	ers::fatal( ex );
    }
  \endcode
  
  \section ERS_HERE ERS_HERE macro
  The macro ERS_HERE is used to insert all the context information to an ers issue. 
  When constructing an issue one should always give the macro ERS_HERE as a first parameter. 
  \see ers::Context

*/
