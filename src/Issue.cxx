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
#include <time.h>

#include <ers/Issue.h>
#include <ers/IssueFactory.h>
#include <ers/OutputStream.h>
#include <ers/ers.h>

using namespace ers;

ERS_DECLARE_ISSUE( ers, StdIssue, , )
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
    m_time( boost::posix_time::second_clock::local_time() )
{
    add_qualifier( m_context->package_name() );
}

/** Constructor - takes another exceptions as the cause for the current exception.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param cause exception that caused the current Issue
 */
Issue::Issue(	const Context & context,
                const std::exception & cause )
  : m_context( context.clone() ),
    m_severity( ers::Error ),
    m_time( boost::posix_time::second_clock::local_time() )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_HERE, cause.what() ) );
    add_qualifier( m_context->package_name() );
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
    m_time( boost::posix_time::second_clock::local_time() )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_HERE, cause.what() ) );
    add_qualifier( m_context->package_name() );
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

// ====================================================
// Field Access Methods
// ====================================================

// message

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

void
Issue::prepend_message( const std::string & msg )
{
    m_message = msg + m_message;
}

// ====================================================
// OutputStream Operators
// ====================================================

#define FIELD_SEPARATOR "\n\t"
namespace ers
{
    
    /** Standard Streaming operator - puts the human description into the OutputStream.
     * \param out the destination OutputStream
     * \param issue the Issue to OutputStream
     */
    std::ostream & operator<<( std::ostream & out, const ers::Issue & issue )
    {
	out << ers::to_string( issue.severity() ) << " " << issue.time() << " ["
	    << issue.context().position() << "] \"" << issue.message() << "\"";
	
	if ( ers::verbosity_level() > 0 )
	{
	    ers::string_map::const_iterator it = issue.parameters().begin();
            for ( ; it != issue.parameters().end(); ++it )
	    {
		out << FIELD_SEPARATOR << it->first << "=\"" << it->second << "\" ";
	    }
        }
	
        if ( ers::verbosity_level() > 1 )
	{
	    out << FIELD_SEPARATOR << "host = " << issue.context().host_name()
		<< FIELD_SEPARATOR << "user = " << issue.context().user_name()
				   << " (" << issue.context().user_id() << ")"
		<< FIELD_SEPARATOR << "process id = " << issue.context().process_id()
		<< FIELD_SEPARATOR << "process wd = " << issue.context().cwd();
	}
        
	if ( ers::verbosity_level() > 2 )
	{
	    std::vector<std::string> stack = issue.context().stack();
	    for( size_t i = 0; i < stack.size(); i++ )
	    {
		out << FIELD_SEPARATOR << "#" << std::setw(3) << std::left << i << stack[i];
	    }
	}
        
        if ( issue.cause() )
        {
            out << FIELD_SEPARATOR << "was caused by: " << *issue.cause();
        }
        
	return out;
    }
}




