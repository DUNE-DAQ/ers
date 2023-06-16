/*
 *  Context.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 26.11.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
#include <string.h>
#include <cxxabi.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#ifndef __rtems__
#include <execinfo.h>
#else
char** backtrace_symbols (void ** , int size) {
    return 0;
}
#endif

#include <ers/Context.hpp>
#include <ers/Configuration.hpp>

namespace
{
    std::string
    demangle( char * mangled )
    {
        int status;
	char * function_begin = ::strchr( mangled, '(' );
        if ( function_begin ) {
            char * function_end = ::strchr( ++function_begin, '+' );
            if ( function_end && function_end != function_begin)
            {
                std::string fname(function_begin, function_end - function_begin);
                char * name = abi::__cxa_demangle( fname.c_str(), 0, 0, &status );

                if (!name) {
                    return std::string( mangled );
                }

                std::ostringstream out;
                out << std::string(mangled, function_begin - mangled) << name << function_end;
                free( name );
                return out.str();
            }
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
    stack.reserve( stack_size() );
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


dunedaq::ersschema::Context
ers::Context::schema(const std::string & session) const {

  dunedaq::ersschema::Context out;
  out.set_cwd( cwd() );
  out.set_file_name( file_name() );
  out.set_function_name( function_name() );
  out.set_host_name( host_name() );
  out.set_line_number( line_number() );
  out.set_package_name( package_name() );
  
  out.set_process_id( process_id() );
  out.set_thread_id( thread_id() );
  out.set_user_id( user_id() );
  out.set_user_name( user_name() );
  out.set_application_name( application_name() );
  out.set_session( session );
  
  return out;    
  }
