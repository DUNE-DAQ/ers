/*
 *  StreamFactory.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <iostream>


#include "ers/StreamFactory.h"
#include "ers/DefaultStream.h"
#include "ers/ers.h"





/** This variable contains the default keys for building the default streams.
  * The default variables are the following
  * <table border=1 cellpadding=2> 
    <tr><th>ers_severity_none</th><td>null:</td></tr>
    <tr><th>ers_debug_0</th><td rowspan="9">cerr:tab</td></tr>
    <tr><th>ers_debug_1</th></tr>
    <tr><th>ers_debug_2</th></tr>
    <tr><th>ers_debug_3</th></tr>
    <tr><th>ers_information</th></tr>
    <tr><th>ers_notification</th></tr>
    <tr><th>ers_warning</th></tr>
    <tr><th>ers_error</th></tr>
    <tr><th>ers_fatal</th></tr>
    <tr><th>ers_severity_max</th><td>null:</td></tr>
    </table>

  */

const char* ers::StreamFactory::DEFAULT_STREAMS[] = {
    "null",                                                         // none
    "default",  "default",  "default", "default",                 // debug levels
    "default",  "default",  "default",                             // information, notification, warning
    "default",  "default",                                         // Errors and Fatals
    "null:"  } ;                                                    // max


ers::StreamFactory *ers::StreamFactory::s_instance = 0 ; 


// Constructors & Instance access methods
// --------------------------------------

/** Constructor - should never be called by user code, use the \c instance() method instead
  * \see instance() 
  */

ers::StreamFactory::StreamFactory() {
   for(int i= static_cast<int> (ers_severity_none);i< static_cast<int> (ers_severity_max);i++) {	
	ers_severity s = static_cast<ers_severity> (i) ; 
       m_streams[s]=0 ;
   } // for*/
} // StreamFactory

/** Copy constructor - disabled, use the \c instance() method instead
  * \see instance() 
  * \throw ers::NotImplemented in all cases
  */

ers::StreamFactory::StreamFactory(const StreamFactory &other) {
    (void) other ; // shut up the compiler 
    ERS_NOT_IMPLEMENTED(); 
} // StreamFactory

/** This method returns the singleton instance. 
  * It should be used for every operation on the factory. 
  * \return a pointer to the singleton instance 
  */

ers::StreamFactory* ers::StreamFactory::instance() {
    if (0==s_instance) {
	s_instance = new StreamFactory(); 
    } // if
    return s_instance ; 
} // instance

/** Dumps all registered types of streams */

void ers::StreamFactory::print_registered() {
    const StreamFactory *factory = instance();
    std::cerr << *factory ; 
} // print_registered

// Static methods
// --------------------------------------


/** Finds the default stream for a given severity.
  * The stream is searched in the default instance
  * \param s the severity 
  * \return the default stream for the severity 
  */

ers::Stream* ers::StreamFactory::get_default_stream(ers_severity s) {
    return instance()->get_stream(s) ; 
} // get_default_stream

/** Searches for the textual key for a given severity. 
  * This key is first searched in the environnement variables,
  * if this fails, the default values are loaded. 
  * The environnement variable should have the same name than the severity
  * with the prefix ERS_ in front. The whole name should be in uppercases.
  * For instance for severity warning, the environnement variable should be
  * \c ERS_WARNING. 
  * \param s the severity 
  * \return the key describing the stream. 
  */

const char* ers::StreamFactory::key_for_severity(ers_severity s) {
    char key_buffer[64] ; 
    snprintf(key_buffer,sizeof(key_buffer),"ERS_%s",ers::Core::to_string(s)) ;
    char *c = &(key_buffer[0]);
    while(*c) {
	*c = toupper(*c) ;
	c++ ;
    } // while
    const char *env = ::getenv(key_buffer) ; 
    if (env!=0) return env ;
    const char* static_key = DEFAULT_STREAMS[s] ; 
    if (static_key) return static_key ; 
    return ers::DefaultStream::KEY ; 
} // key_for_severity

/** Builds a stream from a textual key 
  * The key should have the format \c protocol:path.extension
  * In certain cases, the path will be empty. 
  * For instance to write in XML format to the error stream, the key is: 
  * \c cerr:.xml 
  * \param key the textual key 
  * \note the stream is allocated on the stack, it is the caller's responsibility to delete it.
  */

ers::Stream *ers::StreamFactory::create_stream(const std::string &key) {
    std::string protocol ; 
    std::string uri ; 
    std::string::size_type colon = key.find(":") ; 
    if (colon==std::string::npos) {
    	protocol = key ; 
    } else {
        protocol = key.substr(0,colon-1) ; 
	uri = key.substr(colon) ; 
    } 
    for(stream_factory_collection::const_iterator pos=m_factories.begin();pos!=m_factories.end();++pos) {
	create_stream_callback callback = pos->second; 
	Stream *s = callback(protocol,uri); 
	if (s!=0) return s ; 
    } // for
    return new DefaultStream(); 
} // create_stream

/** Builds a stream for a given severity. 
  * The actual key for the severity is found using \c key_for_severity,
  * then the appropriate stream is constructred using \c create_stream
  * \see key_for_severity()
  * \see create_stream()
  * \param s the severity of the requested stream
  * \return the newly created stream
  * \note the stream is allocated on the stack, it is the caller's responsibility to delete it.
  */

ers::Stream *ers::StreamFactory::create_stream(ers_severity s) {
    const char* key = key_for_severity(s); 
    return create_stream(key); 
} // get_stream


/** Sends an Issue to the fatal error stream 
 * \param issue_ptr 
 */

void ers::StreamFactory::fatal(Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    issue_ptr->severity(ers_fatal); 
    dispatch(issue_ptr,false); 
} // fatal

/** Sends an Issue to the error stream 
 * \param issue_ptr 
 */

void ers::StreamFactory::error(Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    issue_ptr->severity(ers_error); 
    dispatch(issue_ptr,false); 
} // error

/** Sends an Issue to the warning stream 
* \param issue_ptr the issue to send
*/

void ers::StreamFactory::warning(Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    issue_ptr->severity(ers_warning); 
    dispatch(issue_ptr,false); 
} // warning

/** Sends an issue to the debug stream 
 * \param issue_ptr the Issue to send
 * \param s the severity of the associated stream. 
 *          Accepted values: \li \c ers_debug_0 \li \c ers_debug_1 \li \c ers_debug_2 \li \c ers_debug_3 
 */

void ers::StreamFactory::debug(Issue *issue_ptr, ers_severity s) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    ERS_PRECONDITION(s<ers_information,"severity is not debug : %s (%d) %d",ers::Core::to_string(s),s,ers_information);
    issue_ptr->severity(s) ; 
    dispatch(issue_ptr,false);
} //  debug

/** Sends a message to the debug stream 
* \param c the Context of the message
* \param message the text of the message 
* \param s the severity of the associated stream. Accepted values: \li \c ers_debug_0 \li \c ers_debug_1 \li \c ers_debug_2 \li \c ers_debug_3 
*/

void ers::StreamFactory::debug(const Context &c, const std::string &message, ers_severity s) {
    LogIssue log_issue(c,s,message); 
    debug(&log_issue,s); 
} // debug

/** Sends a message to the warning stream 
 * \param c the context of the message
 * \param message the message to send 
 */

void ers::StreamFactory::warning(const Context &c, const std::string &message) {
    LogIssue log_issue(c,ers_warning,message);
    warning(&log_issue); 
} // warning


/** Dispatches an issue to the appropriate stream.
 * The stream is decided based upon the severity specified in the Issue.
 * If \c throw_error is true errors and fatal errors are not sent to a stream, but thrown in the context of the caller.
 * \param issue_ptr the Issue to dispatch
 * \param throw_error should errors and fatals are thrown 
 */

void ers::StreamFactory::dispatch(Issue *issue_ptr, bool throw_error) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    if (throw_error && issue_ptr->is_error()) { throw *issue_ptr  ; } 
    const ers_severity s = issue_ptr->severity() ;
    Stream *stream_ptr =  instance()->get_stream(s) ; 
    ERS_CHECK_PTR(stream_ptr);
    stream_ptr->send(issue_ptr); 
} // dispatch


void ers::StreamFactory::dispatch(Issue &issue_ref, bool throw_error) {
	dispatch(&issue_ref,throw_error); 
} // dispatch


void ers::StreamFactory::set_stream(ers_severity s, const std::string &key) {
    instance()->set(s,key.c_str()) ; 
} // set



// Member methods
// --------------------------------------

/** Gets stream for severity 
  * \param s the severity of the requested stream
  * \return the stream 
  */

ers::Stream * ers::StreamFactory::get_stream(ers_severity s)  {
    if (m_streams[s]==0) {
	m_streams[s]=create_stream(s) ; 
    } // if 
    return m_streams[s] ; 
} // get_stream


/** Sets the stream for a given severity 
  * \param severity  severity of the stream 
  * Accepted values: \li \c ers_debug_0 \li \c ers_debug_1 \li \c ers_debug_2 \li \c ers_debug_3 
  * \li \c ers_information \li \c ers_notification \li \c ers_warning
  * \li \c ers_error \li \c ers_fatal
  * \param s the new stream 
  */

void ers::StreamFactory::set(ers_severity severity, Stream *s) {
    ERS_PRE_CHECK_PTR(s); 
    ERS_PRECONDITION(ers_severity_none < severity && severity < ers_severity_max,"illegal severity %d",(int) severity);
    if (m_streams[severity]) {
	delete m_streams[severity] ;
    } // if there is a stream defined 
    m_streams[severity] = s ; 
} // stream

/** Builds a stream using a string key and sets it up for a given severity
  * \param severity  severity of the stream 
  * Accepted values: \li \c ers_debug_0 \li \c ers_debug_1 \li \c ers_debug_2 \li \c ers_debug_3 
  * \li \c ers_information \li \c ers_notification \li \c ers_warning
  * \li \c ers_error \li \c ers_fatal
  * \param key the key used to build the new stream 
  */

void ers::StreamFactory::set(ers_severity severity, const char* key) {
    ERS_PRE_CHECK_PTR(key); 
    Stream *s = create_stream(key);
    set(severity,s); 
} // 

/** 
  * \return the current fatal stream 
  */

ers::Stream *ers::StreamFactory::fatal()  { return get_stream(ers_fatal) ; } // fatal 

/**
 * \return the current error stream 
 */

ers::Stream* ers::StreamFactory::error()  { return get_stream(ers_error) ; } // error

/** 
 * \return the current warning stream 
 */

ers::Stream* ers::StreamFactory::warning()  { return get_stream(ers_warning); } // warning


/** Finds the debug stream 
 * \param s the severity of the associated stream. 
 * Accepted values: \li \c ers_debug_0 \li \c ers_debug_1 \li \c ers_debug_2 \li \c ers_debug_3 
 * \return debug stream 
 */

ers::Stream* ers::StreamFactory::debug(ers_severity s)  {
    ERS_PRECONDITION(s<ers_information && s>ers_severity_none,"severity is not debug : %s (%d)",ers::Core::to_string(s),s);
    return get_stream(s) ; 
} // debug




/** Registers a factory function with the stream factory.
  * The callback is function that takes two parameters <ol>
  * <li>a string describing the protocol, for instance \c file </li>
  * <li>a string describing a uri, can be a path, a suffix or anything</li>
  * </ol>
  * The function should return a heap allocated stream, or null if it does not
  * understand the protocol. 
  * \param name name of the stream type (human display only).
  * \param callback the callback function
  * \return \c true if the registration was sucessfull 
  */

bool ers::StreamFactory::register_factory(const std::string &name, create_stream_callback callback) {
    m_factories[name] = callback ;
    return true  ;
} // register_factory

/** Writes a description of the factory to a stream. 
  * This method gives a list of all registered stream types. 
  * \param stream the stream to write into
  */

void ers::StreamFactory::write_to(std::ostream& stream) const {
    stream << "Stream factory - registered streams\n" ; 
    stream << "-----------------------------------\n" ; 
    int i = 0 ; 
    for(stream_factory_collection::const_iterator pos=m_factories.begin();pos!=m_factories.end();++pos) {
	std::string name = pos->first; 
	stream << i << ")\t" << name << std::endl; 
	i++ ; 
    } // for
    stream << "-----------------------------------\n" ; 
} // write_to


/** Streaming operator
  * \param stream destination stream
  * \param factory the factory object to display
  * \return the stream passed as first parameter
  * \see ers::StreamFactory::write_to()
  */

std::ostream& ers::operator<<(std::ostream& stream, const ers::StreamFactory& factory) {
    factory.write_to(stream);
    return stream ; 
} // operator




