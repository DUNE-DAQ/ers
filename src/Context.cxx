/*
 *  Context.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 26.11.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
#include <iostream>
#include <sstream>
#include <cxxabi.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <execinfo.h>

#include <ers/Context.h>

namespace
{
    std::string
    readable_type_name( char * mangled )
    {
	int status;
	char * module = mangled;
	char * function_begin = strchr( module, '(' );
	char * function_end = strchr( module, '+' );
	if ( function_begin && function_end )
	{
	    *function_begin++ = 0;
	    *function_end = 0;
	    char * name = abi::__cxa_demangle( function_begin, 0, 0, &status );
	    std::ostringstream out;
	    out << ( name ? name : function_begin ) << " from " << module;
	    if ( name )
		free( name );
	    return out.str();
	}
	return std::string( mangled );
    }
    
    void
    print_function( std::ostream & out, const char * function )
    {
	const char * pos = strchr( function, '(' );
	if ( pos )
	{
	    out.write( function_name(), pos - function );
	    out << "(...)";
	} else {
	    out << function_name();
	}
    }
}

std::vector<std::string>
ers::Context::stack( ) const
{
    std::vector<std::string>	stack;
    char ** symbols = backtrace_symbols( (void**)stack_symbols(), stack_size() );
    
    //////////////////////////////////////////////////////////////////
    // This stack frame is compiler an compilation dependant,
    // we relay on GCC optimized compilation
    // the last frame contains some garbage
    //////////////////////////////////////////////////////////////////
    for (int i = 0; i < stack_size() - 1; i++)
    {
	stack.push_back( readable_type_name( symbols[i] ) );
    }
    free(symbols);
    return stack;
}

/** Pretty printed code position 
  * format: package_name/file_name:line_number <function_name>
  * \return reference to string containing format
  * \note the file name is truncated from the last slash 
  */
std::string
ers::Context::position() const
{
    std::ostringstream out;
    out << "<";
    print_function( out, function_name() );
    out << "> at ";
    
    const char * file = file_name();
    if (    file[0] == '.'
    	&&  file[0] == '.'
        &&  file[0] == '/' ) // file name starts with "../"
    {
	out << package_name() << (file + 2);
    } else {
	out << file;
    }
    out << ":" << line_number();
    return out.str();
}
