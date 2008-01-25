/*
 *  StandardStreamOutput.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.07.
 *  Copyright 2007 CERN. All rights reserved.
 *
 */

#include <ers/Issue.h>
#include <ers/StandardStreamOutput.h>
#include <ers/Severity.h>


#define FIELD_SEPARATOR "\n\t"

std::ostream &
ers::StandardStreamOutput::println( std::ostream & out, const Issue & issue, int verbosity )
{
    print( out, issue, verbosity );
    out << std::endl;
}

std::ostream &
ers::StandardStreamOutput::print( std::ostream & out, const Issue & issue, int verbosity )
{
    if ( verbosity > -3 )
    {
	out << ers::to_string( issue.severity() ) << " ";
    }

    if ( verbosity > -2 )
    {
	out << issue.time() << " ";
    }

    if ( verbosity > -1 )
    {
	out << "[" << issue.context().position( verbosity ) << "] ";
    }

    out << issue.message();

    if ( verbosity > 1 )
    {
	out << FIELD_SEPARATOR << "Parameters = ";
	for ( ers::string_map::const_iterator it = issue.parameters().begin(); it != issue.parameters().end(); ++it )
	{
	    out << "'" << it->first << "=" << it->second << "' ";
	}

	out << FIELD_SEPARATOR << "Qualifiers = ";
	for ( std::vector<std::string>::const_iterator it = issue.qualifiers().begin(); it != issue.qualifiers().end(); ++it )
	{
	    out << "'" << *it << "' ";
	}
    }

    if ( verbosity > 2 )
    {
	out << FIELD_SEPARATOR << "host = " << issue.context().host_name()
	    << FIELD_SEPARATOR << "user = " << issue.context().user_name()
			       << " (" << issue.context().user_id() << ")"
	    << FIELD_SEPARATOR << "process id = " << issue.context().process_id()
	    << FIELD_SEPARATOR << "thread id = " << issue.context().thread_id()
	    << FIELD_SEPARATOR << "process wd = " << issue.context().cwd();
    }

    if ( verbosity > 3 )
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
