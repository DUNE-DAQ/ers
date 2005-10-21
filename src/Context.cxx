/*
 *  Context.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
#include <iostream>
#include <sstream>
#include <cxxabi.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <ers/Context.h>

namespace
{
    const char * const get_cwd( )
    {
	static std::string buf;
	if ( buf.empty() )
	{
	    char tmp[1024];
	    if ( getcwd( tmp, sizeof( tmp ) ) )
		buf = tmp;
	}
	return buf.c_str();
    }

    const char * const get_user_name()
    {
	static std::string buf;
	if ( buf.empty() )
	{
	    struct passwd * psw = getpwuid( geteuid() );
	    if ( psw )
		buf = psw->pw_name;
	}
	return buf.c_str();
    }

    const char * const get_host_name()
    {
	static std::string buf;
	if ( buf.empty() )
	{
	    char tmp[1024];
	    if ( !gethostname( tmp, sizeof( tmp ) ) )
		buf = tmp;
	}
	return buf.c_str();
    }

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
	    out << ( name ? name : function_begin ) << "() from " << module;
	    if ( name )
		free( name );
	    return out.str();
	}
	return std::string( mangled );
    }
}


const ers::Context		ers::Context::EmptyInstance; 
const ers::ProcessContext	ers::Context::c_process( get_host_name(), 
							 getpid(), 
                                                	 get_cwd(), 
                                                	 geteuid(), 
                                                	 get_user_name() );

std::vector<std::string>
ers::Context::stack( ) const
{
    std::vector<std::string>	stack;
    char ** symbols = backtrace_symbols( m_stack, m_stack_size );
    
    //
    // This stack frame is compiler an compilation dependant, we relay on GCC optimized compilation
    // the last frame contains some garbage
    //
    for (int i = 0; i < m_stack_size - 1; i++)
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
std::string ers::Context::position() const {
    std::ostringstream out;
    char * pos = strchr( m_file_name, '/' );
    out << "<" << m_function_name << "> at ";
    out << m_compiler.m_package << "/";
    if ( !pos ) {
	out << m_file_name;
    } else {
	out << ++pos;
    } //
    out << ":" << m_line_number;
    return out.str();
} // position

/** Pretty printed compiler name 
  * \return reference to string containing format 
  */

std::string ers::Context::compiler() const
{
    std::ostringstream out;
    out << m_compiler.m_name << " " << m_compiler.m_version;
    return out.str();
} // compiler

/** Pretty printed compilation time description
  * \return reference to string containing description
  */

std::string ers::Context::compiled_at() const
{
    std::ostringstream out;
    out << m_compiler.m_date << " " << m_compiler.m_time;
    return out.str();
} // compiled_at
