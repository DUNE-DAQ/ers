/*
 *  Issue.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
 
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include <boost/date_time/c_local_time_adjustor.hpp>

#include <ers/Issue.h>
#include <ers/IssueFactory.h>
#include <ers/OutputStream.h>
#include <ers/StandardStreamOutput.h>
#include <ers/ers.h>
#include <ers/internal/Util.h>

using namespace ers;

ERS_DECLARE_ISSUE( ers, StdIssue, , )

namespace
{
    int get_default_qualifiers( std::vector<std::string> & qualifiers )
    {
    	static const char * environment = ::getenv( "TDAQ_ERS_QUALIFIERS" );
        if ( environment )
        {
	    ers::tokenize( environment, ",", qualifiers );
        }
        return 1;
    }
    
    void add_default_qualifiers( Issue & issue )
    {
    	static std::vector<std::string> qualifiers;
        static int _dummy_ = get_default_qualifiers( qualifiers );
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


/** Constructor - takes another exceptions as the cause for the current exception.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param message the user message associated with this issue
 */
Issue::Issue(	const Context & context,
		const std::string & message )
  : m_cause( 0 ),
    m_context( context.clone() ),
    m_message( message ),
    m_severity( ers::Error ),
    m_time( boost::posix_time::second_clock::universal_time() )
{
    add_qualifier( m_context->package_name() );
    add_default_qualifiers( *this );
}

/** Constructor - takes another exceptions as the cause for the current exception.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param cause exception that caused the current Issue
 */
Issue::Issue(	const Context & context,
                const std::exception & cause )
  : m_context( context.clone() ),
    m_severity( ers::Error ),
    m_time( boost::posix_time::second_clock::universal_time() )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_HERE, cause.what() ) );
    add_qualifier( m_context->package_name() );
    add_default_qualifiers( *this );
}

/** Constructor - takes another exceptions as the cause for the current exception.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param message the user message associated with this issue
 * \param cause exception that caused the current Issue
 */
Issue::Issue(	const Context & context,
		const std::string & message,
		const std::exception & cause )
  : m_context( context.clone() ),
    m_message( message ),
    m_severity( ers::Error ),
    m_time( boost::posix_time::second_clock::universal_time() )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_HERE, cause.what() ) );
    add_qualifier( m_context->package_name() );
    add_default_qualifiers( *this );
}

ers::Issue::~Issue() throw()
{ ; }

std::string 
ers::Issue::time() const
{
    return boost::posix_time::to_simple_string( 
    	boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local( m_time )  );
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

/** Add a qualifier to the qualifier list
  * \param qualif the qualifier to add
  */
void 
Issue::add_qualifier( const std::string & qualifier )
{
    if ( std::find( m_qualifiers.begin(), m_qualifiers.end(), qualifier ) == m_qualifiers.end() )
	m_qualifiers.push_back( qualifier );
}

ers::Severity
Issue::set_severity( ers::Severity severity ) const
{
    ers::Severity old_severity = m_severity;
    m_severity = severity;
    return old_severity;
}

/** Add the given text to the beginning of the current issue's message
  * \param msg text to be prepended
  */
void
Issue::prepend_message( const std::string & msg )
{
    m_message = msg + m_message;
}

/** Add the given text strings to the beginning and to the end of the current issue's message
  * \param begin text to be prepended
  * \param begin text to be appended
  */
void
Issue::wrap_message( const std::string & begin, const std::string & end )
{
    m_message = begin + m_message + end;
}

namespace ers
{   
    /** Standard streaming operator - puts the issue in human readable format into the OutputStream.
     * \param out the destination OutputStream
     * \param issue the Issue to be printed
     */
    std::ostream & operator<<( std::ostream & out, const ers::Issue & issue )
    {
        return StandardStreamOutput::print( out, issue, ers::verbosity_level() );
    }
}




