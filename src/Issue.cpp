/*
 *  Issue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
 
#include <csignal>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <time.h>

#include <ers/Issue.hpp>
#include <ers/IssueFactory.hpp>
#include <ers/OutputStream.hpp>
#include <ers/StandardStreamOutput.hpp>
#include <ers/ers.hpp>
#include <ers/internal/Util.hpp>

using namespace ers;

ERS_DECLARE_ISSUE( ers, StdIssue, ERS_EMPTY, ERS_EMPTY )

namespace
{
    int get_default_qualifiers( std::vector<std::string> & qualifiers )
    {
        static const char * environment = ::getenv( "DUNEDAQ_ERS_QUALIFIERS" );
        if ( environment )
        {
        ers::tokenize( environment, ",", qualifiers );
        }
        return 1;
    }
    
    void add_default_qualifiers( Issue & issue )
    {
        static std::vector<std::string> qualifiers;
        // static int _unused_ = get_default_qualifiers( qualifiers );
        get_default_qualifiers( qualifiers );
    for ( std::vector<std::string>::const_iterator it = qualifiers.begin(); it != qualifiers.end(); ++it )
        {
        issue.add_qualifier( *it );
        }
    }    
}

Issue::Issue( const Issue & other )
  : std::exception( other ),
    m_cause( other.m_cause.get() ? other.m_cause->clone() : 0 ),
    m_context( other.m_context->clone() ),
    m_message( other.m_message ),
    m_qualifiers( other.m_qualifiers ),
    m_severity( other.m_severity ),
    m_time( other.m_time ),
    m_values( other.m_values )
{ ; }


/** This constructor create a new issue with the given message.
 * \param context the context of the Issue, e.g where in the code the issue appeared
 * \param message the user message associated with this issue
 */
Issue::Issue(   const Context & context,
        const std::string & message )
  : m_context( context.clone() ),
    m_message( message ),
    m_severity( ers::Error ),
    m_time( system_clock::now() )
{
    add_qualifier( m_context->package_name() );
    add_default_qualifiers( *this );
}

/** This constructor takes another exceptions as its cause.
 * \param context the context of the Issue, e.g where in the code the issue appeared
 * \param cause the other exception that has caused this one
 */
Issue::Issue(   const Context & context,
                const std::exception & cause )
  : m_context( context.clone() ),
    m_severity( ers::Error ),
    m_time( system_clock::now() )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_HERE, cause.what() ) );
    add_qualifier( m_context->package_name() );
    add_default_qualifiers( *this );
}

/** This constructor takes another exceptions as its cause.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param message the user message associated with this issue
 * \param cause exception that caused the current issue
 */
Issue::Issue(   const Context & context,
        const std::string & message,
        const std::exception & cause )
  : m_context( context.clone() ),
    m_message( message ),
    m_severity( ers::Error ),
    m_time( system_clock::now() )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_HERE, cause.what() ) );
    add_qualifier( m_context->package_name() );
    add_default_qualifiers( *this );
}

Issue::Issue(   Severity severity,
        const system_clock::time_point & time,
                const ers::Context & context,
        const std::string & message,
        const std::vector<std::string> & qualifiers,
        const std::map<std::string, std::string> & parameters,
        const ers::Issue * cause )
  : m_cause( cause ),
    m_context( context.clone() ),
    m_message( message ),
    m_qualifiers( qualifiers ),
    m_severity( severity ),
    m_time( time ),
    m_values( parameters )
{ ; }

ers::Issue::~Issue() noexcept
{ ; }

std::time_t 
ers::Issue::time_t() const
{
    return system_clock::to_time_t(m_time);
}

void 
ers::Issue::get_value( const std::string & key, const char * & value ) const
{
    string_map::const_iterator it = m_values.find(key);
    if ( it != m_values.end() )
    {
    value = it->second.c_str();
    }
    else
    {
        throw ers::NoValue( ERS_HERE, key );
    }
}

void 
ers::Issue::get_value( const std::string & key, std::string & value ) const
{
    string_map::const_iterator it = m_values.find(key);
    if ( it != m_values.end() )
    {
    value = it->second;
    }
    else
    {
        throw ers::NoValue( ERS_HERE, key );
    }
}

/** Add a new qualifier to the qualifiers list of this issue
  * \param qualifier the qualifier to add
  */
void 
Issue::add_qualifier( const std::string & qualifier )
{
    if ( std::find( m_qualifiers.begin(), m_qualifiers.end(), qualifier ) == m_qualifiers.end() ) {
        m_qualifiers.push_back( qualifier );
    }
}

ers::Severity
Issue::set_severity( ers::Severity severity ) const
{
    ers::Severity old_severity = m_severity;
    m_severity = severity;
    return old_severity;
}

/** Adds the given text to the beginning of the issue's message
  * \param msg text to be prepended
  */
void
Issue::prepend_message( const std::string & msg )
{
    m_message = msg + m_message;
}

/** Adds the given text strings to the beginning and to the end of the issue's message
  * \param begin text to be prepended
  * \param begin text to be appended
  */
void
Issue::wrap_message( const std::string & begin, const std::string & end )
{
    m_message = begin + m_message + end;
}

namespace ers {
  
  dunedaq::ersschema::IssueChain ToChain( const Issue & i) {

    dunedaq::ersschema::IssueChain out;

    (* out.mutable_message()) = ToObject(i);

    auto cause = i.cause();

    while ( cause ) {
      auto ptr = i.add_causes() ;
      *ptr = ToObject( *cause );
      cause = cause -> cause();
    }

    return out;    
  }

  dunedaq::ersschema::IssueObject ToObject( const Issue & i ) {

    auto c = ToObject( i.context() );
    
    dunedaq::ersschema::IssueObject out;

    (*out.mutable_context())=c;

    out.set_name( i.get_class_name() );
    out.set_message( i.message() ) ;
    out.set_severity( std::to_string( i.severity() ) );
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(i.ptime().time_since_epoch()).count();
    out.set_time(time);
    
    auto & params = (* out.mutable_parameters());
    for ( auto p : i.parameters() ) {
      params[p.first] = p.second;
    }
    
    return out;    
  }

  dunedaq::ersschema::Context ToObject( const Context & c) {

    dunedaq::ersschema::Context out;
    out.set_cwd( c.cwd() );
    out.set_file_name( c.file_name() );
    out.set_function_name( c.function_name() );
    out.set_host_name( c.host_name() );
    out.set_line_number( c.line_number() );
    out.set_package_name( c.package_name() );

    out.set_process_id( c.process_id() );
    out.set_thread_id( c.thread_id() );
    out.set_user_id( c.user_id() );
    out.set_user_name( c.user_name() );
    out.set_application_name( c.application_name() );
    
    return out;    
  }

  

  /** Standard streaming operator - puts the issue in human readable format into the standard out stream.
     * \param out the destination out stream
     * \param issue the Issue to be printed
     */
    std::ostream & operator<<( std::ostream & out, const ers::Issue & issue )
    {
        return StandardStreamOutput::print( out, issue, ers::verbosity_level() );
    }
}




