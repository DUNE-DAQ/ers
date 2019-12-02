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

#ifndef __rtems__
#include <execinfo.h>
#else
char** backtrace_symbols (void ** , int size) {
    return 0;
}
#endif

#include <ers/Context.h>
#include <ers/Configuration.h>

namespace
{
    std::string
    demangle( char * mangled )
    {
        int status;
	char * function_begin = ::strchr( mangled, '(' );
	char * function_end = ::strchr( mangled, '+' );
	if ( function_begin && function_end )
	{
	    *function_begin++ = 0;
	    *function_end++ = 0;
	    char * name = abi::__cxa_demangle( function_begin, 0, 0, &status );
            std::ostringstream out;
            out << mangled << "(" << (name ? name : function_begin) << "+" << function_end;
            free( name );
            return out.str();
	}
	return std::string( mangled );
    }
    
    void
    print_function( std::ostream & out, const char * function, int verbosity )
    {
	if ( verbosity )
        {
	    out << function;
            return;
        }
        
        const char * end = strchr( function, '(' );
	if ( end )
	{
	    const char * beg = end;
	    while ( beg > function ) {
	        if ( *(beg-1) == ' ' ) {
	            break;
	        }
	        --beg;
	    }
            out.write( beg, end - beg );
            out << "(...)";
	} else {
	    out << function;
	}
    }
}

std::vector<std::string>
ers::Context::stack( ) const
{
    std::vector<std::string>	stack;
    char ** symbols = backtrace_symbols( (void**)stack_symbols(), stack_size() );
    
    if (symbols) {
        for (int i = 1; i < stack_size(); i++) {
            stack.push_back( demangle(symbols[i]) );
        }
        free(symbols);
    }

    return stack;
}

/** Pretty printed code position 
  * format: package_name/file_name:line_number <function_name>
  * \return reference to string containing format
  */
std::string
ers::Context::position( int verbosity ) const
{
    std::ostringstream out;
    print_function( out, function_name(), verbosity );
    out << " at ";
    
    const char * file = file_name();
    if (    file[0] == '.'
    	&&  file[1] == '.'
        &&  file[2] == '/' ) // file name starts with "../"
    {
	out << package_name() << (file + 2);
    } else {
	out << file;
    }
    out << ":" << line_number();
    return out.str();
}
