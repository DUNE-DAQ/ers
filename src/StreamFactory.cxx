/*
 *  StreamFactory.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/StreamFactory.h"

#include "ers/HumanStream.h"
#include "ers/TabOutStream.h"
#include "ers/FIFOStream.h"

#ifdef USE_TINY_XML
#include "ers/TinyXMLStream.h"
#else 
#include "ers/XercesStream.h"
#endif

#include "ers/InvalidReferenceIssue.h"
#include "ers/Precondition.h"
#include "ers/LogIssue.h"


const char *ers::StreamFactory::FILE_KEY = "file" ; 
const char *ers::StreamFactory::NULL_STREAM_KEY = "null" ;
const char *ers::StreamFactory::CERR_STREAM_KEY = "cerr" ; 
const char *ers::StreamFactory::FIFO_STREAM_KEY = "fifo" ;

const char *ers::StreamFactory::XML_SUFFIX = "xml" ; 
const char *ers::StreamFactory::TAB_SUFFIX = "tab" ; 

const char* ers::StreamFactory::DEFAULT_STREAMS[] = {
    "cerr:.tab", "cerr:.tab", "cerr:.tab", "cerr:.tab", "cerr:.tab", // debug levels
    "cerr:.tab", "cerr:.tab", "cerr:.tab", "cerr:.tab",              // information, notification, warning
    "cerr:.tab", "fifo:",                                            // Errors and Fatals
    "null:", "null:" } ;                                             // None & max


ers::StreamFactory *ers::StreamFactory::s_instance = 0 ; 


// Constructors & Instance access methods
// --------------------------------------

ers::StreamFactory::StreamFactory() {
    for(int i= static_cast<int> (ers_severity_none);i< static_cast<int> (ers_severity_max);i++) {	
	ers_severity s = static_cast<ers_severity> (i) ; 
	m_streams[s]=create_stream(s) ; 
    } // for
} // StreamFactory

ers::StreamFactory* ers::StreamFactory::instance() {
    if (0==s_instance) {
	s_instance = new StreamFactory(); 
    } // if
    return s_instance ; 
} // instance

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
    snprintf(key_buffer,sizeof(key_buffer),"ERS_%s",get_severity_text(s)) ;
    char *c = &(key_buffer[0]);
    while(*c) {
	*c = toupper(*c) ;
	c++ ;
    } // while
    const char *env = ::getenv(key_buffer) ; 
    if (env!=0) return env ;
    const char* static_key = DEFAULT_STREAMS[s] ; 
    if (static_key) return static_key ; 
    return CERR_STREAM_KEY ; 
} // key_for_severity

/** Builds a stream from a textual key 
  * The key should have the format \c protocol:path.extension
  * In certain cases, the path will be empty. 
  * For instance to write in XML format to the error stream, the key is: 
  * \c cerr:.xml 
  * \param c_key the textual key 
  * \note the stream is allocated on the stack, it is the caller's responsibility to delete it.
  */

ers::Stream *ers::StreamFactory::create_stream(const char* c_key) {
    if (c_key==0) return new Stream(); 
    std::string key = std::string(c_key); 
    std::string protocol = System::File::protocol(key);
    if (protocol==FILE_KEY) return factory(System::File::from_url(key));
    if (protocol==NULL_STREAM_KEY) return new Stream(); 
    if (protocol==CERR_STREAM_KEY) return factory(&std::cerr,System::File::uri(key)) ; 
    if (protocol==FIFO_STREAM_KEY) return new FIFOStream() ; 
    return new HumanStream(&std::cerr); 
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
    ERS_PRECONDITION(s<ers_information,"severity is not debug : %s (%d) %d",get_severity_text(s),s,ers_information);
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


/** This function builds a stream that writes into a STL stream
 * The streaming format is decide by parameter \c type
 * \param s the stream to stream into
 * \param type the type of streaming (xml or tab). 
 * \note XML is not implemented 
 */

ers::Stream* ers::StreamFactory::factory(std::ostream *s, const std::string &type)  {
    if (type==TAB_SUFFIX) return new TabOutStream(s); 
    return new TabOutStream(s); 
} // factory

/** This function builds a streams that writes into a file.
 * The streaming format is decided by the suffix of the file. 
 * \param file File to write into 
 */

ers::Stream* ers::StreamFactory::factory(const System::File &file) {
    if (file.extension()==XML_SUFFIX) {
    	return new ers::XercesStream(file); 
    } // XML
    if (file.extension()==TAB_SUFFIX) {
        return new TabOutStream(file); 
    } // TAB
    return new HumanStream(file); 
} // factory


// Member methods
// --------------------------------------

/** Gets stream for severity 
  * \param severity the severity of the requested stream
  * \return the stream 
  */


ers::Stream * ers::StreamFactory::get_stream(ers_severity s) const {
    return m_streams[s] ; 
} // get_stream

/** 
  * \return the current fatal stream 
  */

ers::Stream *ers::StreamFactory::fatal() const { return get_stream(ers_fatal) ; } // fatal 

/**
 * \return the current error stream 
 */

ers::Stream* ers::StreamFactory::error() const { return get_stream(ers_error) ; } // error

/** 
 * \return the current warning stream 
 */

ers::Stream* ers::StreamFactory::warning() const { return get_stream(ers_warning); } // warning


/** Finds the debug stream 
 * \param s the severity of the associated stream. 
 * Accepted values: \li \c ers_debug_0 \li \c ers_debug_1 \li \c ers_debug_2 \li \c ers_debug_3 
 * \return debug stream 
 */

ers::Stream* ers::StreamFactory::debug(ers_severity s) const {
    ERS_PRECONDITION(s<ers_information && s>ers_severity_none,"severity is not debug : %s (%d)",get_severity_text(s),s);
    return get_stream(s) ; 
} // debug








