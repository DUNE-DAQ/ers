/*
 *  FormattedStandardStream.i
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.07.
 *  Copyright 2007 CERN. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <boost/thread/mutex.hpp>

#define FIELD_SEPARATOR "\n\t"
#define DELIMITER ','

template <class Device>
typename ers::FormattedStandardStream<Device>::Fields	ers::FormattedStandardStream<Device>::supported_fields_;

template <class Device>
ers::FormattedStandardStream<Device>::Fields::Fields( )
{
    (*this)["severity"] = format::Severity;
    (*this)["time"] = format::Time;
    (*this)["position"] = format::Position;
    (*this)["context"] = format::Context;
    (*this)["pid"] = format::PID;
    (*this)["tid"] = format::TID;
    (*this)["user"] = format::User;
    (*this)["cwd"] = format::CWD;
    (*this)["function"] = format::Function;
    (*this)["line"] = format::Line;
    (*this)["text"] = format::Text;
    (*this)["stack"] = format::Stack;
    (*this)["cause"] = format::Cause;
    (*this)["parameters"] = format::Parameters;
    (*this)["qualifiers"] = format::Qualifiers;
}

template <class Device>
std::string
ers::FormattedStandardStream<Device>::get_file_name( const std::string & param )
{
    std::string first_token = param.substr( 0, param.find( ',' ) );
    std::map<std::string,ers::format::Token>::iterator it = supported_fields_.find( first_token );
    if ( it != supported_fields_.end() )
    	return "";
    else
    	return first_token;
}

template <class Device>
std::string
ers::FormattedStandardStream<Device>::get_format( const std::string & param )
{
    std::string first_token = param.substr( 0, param.find( ',' ) );
    std::map<std::string,ers::format::Token>::iterator it = supported_fields_.find( first_token );
    if ( it != supported_fields_.end() )
    	return param;
    else
    	return param.substr( first_token.size() < param.size() ? first_token.size() + 1 : param.size() );
}

template <class Device>
ers::FormattedStandardStream<Device>::FormattedStandardStream( const std::string & param )
  : Device( get_file_name( param ) )
{
    std::string format = get_format( param );
    
    size_t offset = 0;
    size_t index = std::string::npos;
    do {
        index = format.find( DELIMITER, offset );
        std::string token = format.substr( offset, index - offset );
	std::map<std::string,ers::format::Token>::iterator it = supported_fields_.find( token );
	if ( it != supported_fields_.end() )
	    tokens_.push_back( it->second );
	else
	    ;// throw exception here
        
        offset += token.size() + 1;
    } while ( index != std::string::npos );
}

template <class Device>
void
ers::FormattedStandardStream<Device>::report( std::ostream & out, const Issue & issue )
{
    for ( size_t i = 0; i < tokens_.size(); i++ )
    {
	switch ( tokens_[i] )
        {
	    case format::Severity:
		out << ers::to_string( issue.severity() );
                break;
	    case format::Time:
		out << issue.time() << " ";
                break;
	    case format::Position:
		out << "[" << issue.context().position( ) << "]";
                break;
	    case format::Function:
		out << issue.context().function_name();
                break;
	    case format::Line:
		out << issue.context().line_number();
                break;
	    case format::Text:
		out << issue.message();
                break;
	    case format::Parameters:
		{
		    out << FIELD_SEPARATOR << "Parameters = ";
		    for ( ers::string_map::const_iterator it = issue.parameters().begin(); it != issue.parameters().end(); ++it )
		    {
			out << "'" << it->first << "=" << it->second << "' ";
		    }
		}
		break;
	    case format::Qualifiers:
		{
		    out << FIELD_SEPARATOR << "Qualifiers = ";
		    for ( std::vector<std::string>::const_iterator it = issue.qualifiers().begin(); it != issue.qualifiers().end(); ++it )
		    {
			out << "'" << *it << "' ";
		    }
		}
		break;
	    case format::Context:
		out << FIELD_SEPARATOR << "host = " << issue.context().host_name()
		    << FIELD_SEPARATOR << "user = " << issue.context().user_name()
				       << " (" << issue.context().user_id() << ")"
		    << FIELD_SEPARATOR << "process id = " << issue.context().process_id()
		    << FIELD_SEPARATOR << "thread id = " << issue.context().thread_id()
		    << FIELD_SEPARATOR << "process wd = " << issue.context().cwd();
		break;
	    case format::Host:
		out << FIELD_SEPARATOR << "host = " << issue.context().host_name();
                break;
            case format::User:
		out << FIELD_SEPARATOR << "user = " << issue.context().user_name()
				       << " (" << issue.context().user_id() << ")";
                break;
            case format::PID:
		out << FIELD_SEPARATOR << "process id = " << issue.context().process_id();
                break;
            case format::TID:
		out << FIELD_SEPARATOR << "thread id = " << issue.context().thread_id();
                break;
            case format::CWD:
		out << FIELD_SEPARATOR << "process wd = " << issue.context().cwd();
		break;
	    case format::Stack:
		{
		    std::vector<std::string> stack = issue.context().stack();
		    for( size_t i = 0; i < stack.size(); i++ )
		    {
			out << FIELD_SEPARATOR << "#" << std::setw(3) << std::left << i << stack[i];
		    }
		} 
		break;
	    case format::Cause:
		if ( issue.cause() )
		{
		    out << FIELD_SEPARATOR << "was caused by: ";
		    report( out, *issue.cause() );
		}
            default:
            	break;
	}
	out << " ";
    }
    out << std::endl;
}

template <class Device>
void
ers::FormattedStandardStream<Device>::write( const Issue & issue )
{
    report( device().stream(), issue );
    chained().write( issue );
}
