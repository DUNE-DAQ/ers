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

// Constructors
// ====================================================
Issue::Time::Time()
{
    time_t now;
    ::time(&now); 
    char time_buffer[256];
    ctime_r( &now, time_buffer );
    char * cr = strchr(time_buffer,'\n');
    if (cr) {
	*cr = '\0' ;
    } // carriage return 
    time_ = time_buffer;
}

/** Constructor - takes another exceptions as the cause for the current exception.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param message the user message associated with this issue
 * \param cause_exception the exception that caused the current Issue
 */
Issue::Issue(	const Context & context,
		const std::string & message )
  : m_cause( 0 ),
    m_context( context ),
    m_message( message ),
    m_severity( ers::Error )
{
}

/** Constructor - takes another exceptions as the cause for the current exception.
 * \param context the context of the Issue, e.g where in the code did the issue appear  
 * \param message the user message associated with this issue
 * \param cause_exception the exception that caused the current Issue
 */
Issue::Issue(	const Context & context,
                const std::exception & cause )
  : m_context( context ),
    m_severity( ers::Error )
{
    const Issue * issue = dynamic_cast<const Issue *>( &cause );
    m_cause.reset( issue ? issue->clone() : new StdIssue( ERS_EMPTY, cause.what() ) );
    m_qualifiers.push_back( m_context.m_compiler.m_package );
}

/** Destructor. 
  * If the \c m_cause pointer is not null, the pointed Issue is deleted 
  */
Issue::~Issue() throw()
{
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

/**
  * \return the cause of the issue, if there is one, a null pointer otherwise 
  */
const Issue * Issue::cause() const
{
    return m_cause.get();
} // cause

// ====================================================
// Field Access Methods
// ====================================================

/** Gets the severity of the Issue 
 * @return severity of the Issue 
 */

Severity
Issue::severity() const
{
    return m_severity;
} // severity

/** This method overides the what method of the std::exception class.
  * As this method is declared const, it has to use a pre-calculated string
  * @return C style string
  */
const char *
Issue::what() const throw()
{
    return m_message.c_str(); 
}

const std::string &
Issue::message() const
{
    return m_message;
} // message

void
Issue::set_message( const std::string & message )
{
    m_message = message;
} // message

/** Add a qualifier to the qualifier list
  * \param qualif the qualifier to add
  */
void 
Issue::add_qualifier( const std::string & qualifier )
{
    if ( std::find( m_qualifiers.begin(), m_qualifiers.end(), qualifier ) == m_qualifiers.end() )
	m_qualifiers.push_back( qualifier );
}

/** Gets the list of qualifiers 
  * \return list of qualifiers 
  */
const std::vector<std::string> &
Issue::qualifiers() const
{
    return m_qualifiers;
} // qualifiers

const Issue::string_map &
Issue::parameters() const
{
    return m_values;
}

/** Gets the string representation of this issue creation time
  * \return time string
  */
std::string
Issue::time() const
{
    return m_time.time_; 
} // qualifiers

ers::Severity
Issue::set_severity( ers::Severity severity ) const
{
   ers::Severity old_severity = m_severity;
   m_severity = severity;
   return old_severity;
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
	    << issue.m_context.position() << "] \"" << issue.message() << "\"";
	
	if ( ers::verbosity_level() > 0 )
	{
	    for ( ers::Issue::string_map::const_iterator it = issue.m_values.begin(); it != issue.m_values.end(); ++it )
	    {
		out << FIELD_SEPARATOR << it->first << "=\"" << it->second << "\" ";
	    }
        
	    if ( ers::verbosity_level() > 1 )
	    {
		out << FIELD_SEPARATOR << "host = " << issue.m_context.c_process.m_host_name
		    << FIELD_SEPARATOR << "user = " << issue.m_context.c_process.m_user_name
				       << " (" << issue.m_context.c_process.m_user_id << ")"
		    << FIELD_SEPARATOR << "process id = " << issue.m_context.c_process.m_process_id
		    << FIELD_SEPARATOR << "process wd = " << issue.m_context.c_process.m_cwd;

		if ( ers::verbosity_level() > 2 )
		{
		    std::vector<std::string> stack = issue.m_context.stack();
		    for( size_t i = 0; i < stack.size(); i++ )
		    {
			out << FIELD_SEPARATOR << "#" << std::setw(3) << std::left << i << stack[i];
		    }
		}
            }
	}
        
        if ( issue.cause() )
        {
            out << FIELD_SEPARATOR << "was caused by: " << *issue.cause();
        }
        
	return out;
    }
}




