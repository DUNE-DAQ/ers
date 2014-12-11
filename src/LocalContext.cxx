/*
 *  LocalContext.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 26.11.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>

#include <ers/LocalContext.h>

#if !defined(__APPLE__) && !defined(__rtems__)
#include <sys/syscall.h>
#include <execinfo.h>
#else
int backtrace(void**, int) {
    return 0;
}
#endif

pid_t gettid() {
#if !defined(__APPLE__) && !defined(__rtems__)
    return syscall( SYS_gettid );
#else
    return 0;
#endif
}


namespace
{
    const char * get_cwd( )
    {
	static std::string buf;
	if ( buf.empty() )
	{
	    char tmp[1024];
	    if ( ::getcwd( tmp, sizeof( tmp ) ) )
		buf = tmp;
	}
	return buf.c_str();
    }

    const char * get_user_name()
    {
	static std::string buf;
	if ( buf.empty() )
	{
	    struct passwd * psw = ::getpwuid( geteuid() );
	    if ( psw )
		buf = psw->pw_name;
	}
	return buf.c_str();
    }

    const char * get_host_name()
    {
	static std::string buf;
	if ( buf.empty() )
	{
	    char tmp[1024];
	    if ( !::gethostname( tmp, sizeof( tmp ) ) )
		buf = tmp;
	}
	return buf.c_str();
    }
}


ers::LocalContext::LocalContext(
    const char * package_name,
    const char * filename,
    int line_number,
    const char * function_name )
  : m_package_name( package_name ),
    m_file_name( filename ),
    m_function_name( function_name ),
    m_line_number( line_number ),
    m_thread_id( gettid() ),
#ifndef ERS_NO_DEBUG
    m_stack_size( backtrace( m_stack, 128 ) )
#else
    m_stack_size( 0 )
#endif
{ ; }

const char *
ers::LocalContext::application_name() const
{
    static int pid = ::getpid();
    static const char * env = ::getenv( "TDAQ_APPLICATION_NAME" ) ? ::getenv( "TDAQ_APPLICATION_NAME" ) : "Undefined";

    if (pid != ::getpid()) {
    	pid = ::getpid();
	env = ::getenv( "TDAQ_APPLICATION_NAME" ) ? ::getenv( "TDAQ_APPLICATION_NAME" ) : "Undefined";
    }
    return env;
}

const ers::LocalProcessContext	ers::LocalContext::c_process(	get_host_name(),
                                                	 	get_cwd(), 
                                                	 	geteuid(), 
                                                	 	get_user_name());
