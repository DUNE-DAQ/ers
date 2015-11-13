/*
 *  StandardStreamOutput.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.07.
 *  Copyright 2007 CERN. All rights reserved.
 *
 */
#include <ios>

#include <ers/Issue.h>
#include <ers/StandardStreamOutput.h>
#include <ers/Severity.h>
#include <ers/internal/Util.h>

#include <boost/algorithm/string.hpp>

#define FIELD_SEPARATOR "\n\t"

namespace
{    
    const char * const format = ers::read_from_environment(
    		"TDAQ_ERS_TIMESTAMP_FORMAT", "%Y-%b-%d %H:%M:%S");

    const bool isUTC = ::getenv("TDAQ_ERS_TIMESTAMP_UTC");

    std::string timeToStringNano( const ers::Issue & issue ) {
        return issue.time<std::chrono::nanoseconds>(format, isUTC);
    }

    std::string timeToStringMicro( const ers::Issue & issue ) {
        return issue.time<std::chrono::microseconds>(format, isUTC);
    }

    std::string timeToStringMilli( const ers::Issue & issue ) {
        return issue.time<std::chrono::milliseconds>(format, isUTC);
    }

    std::string timeToString( const ers::Issue & issue ) {
        return issue.time(format, isUTC);
    }
    
    std::function<std::string( const ers::Issue & issue )>
    getTimeFormatter()
    {
	static std::string env =
        	::getenv( "TDAQ_ERS_TIMESTAMP_PRECISION" )
                	? ::getenv( "TDAQ_ERS_TIMESTAMP_PRECISION" )
                        : "MILLI";
                        
	boost::algorithm::to_upper(env);

        if ( boost::algorithm::find_first(env, "NANO") )
            return timeToStringNano;
 
        if ( boost::algorithm::find_first(env, "MICRO") )
            return timeToStringMicro;

        if ( boost::algorithm::find_first(env, "MILLI") )
            return timeToStringMilli;

	return timeToString;
    }
    
    const std::function<std::string( const ers::Issue & issue )> put_time = getTimeFormatter();    
}

std::ostream &
ers::StandardStreamOutput::println( std::ostream & out, const Issue & issue, int verbosity )
{
    print( out, issue, verbosity );
    out << std::endl;
    return out;
}

std::ostream &
ers::StandardStreamOutput::print( std::ostream & out, const Issue & issue, int verbosity )
{
    if ( verbosity > -3 )
    {
	out << put_time( issue ) << " ";
    }

    if ( verbosity > -2 )
    {
	out << ers::to_string( issue.severity() ) << " ";
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
	std::ios_base::fmtflags flags( out.flags() );

	out << std::left;
        std::vector<std::string> stack = issue.context().stack();
	for( size_t i = 0; i < stack.size(); i++ )
	{
	    out << FIELD_SEPARATOR << "#" << std::setw(3) << i << stack[i];
	}
        // restore the original state
	out.flags( flags );
    }

    if ( issue.cause() )
    {
	out << FIELD_SEPARATOR << "was caused by: " << *issue.cause();
    }
    
    return out;
}
