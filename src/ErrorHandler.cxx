/*
 *  ErrorHandler.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2ERN. All rights reserved.
 *
 */

#include <csignal>
#include <map>
#include <iomanip>
#include <iostream>

#ifdef ERS_NO_DEBUG
#undef ERS_NO_DEBUG
#endif

#include <ers/Issue.h>
#include <ers/ers.h>


ERS_DECLARE_ISSUE(	ers, 
			UnhandledException,
                        "Unhandled '" << name << "' exception was thrown",
                        ((const char *)name) )

ERS_DECLARE_ISSUE(	ers, 
			SignalCatched,
                        "Got signal " << signum << " " << name,
                        ((int)signum)
                        ((const char *)name) )

namespace ers
{    
    class ErrorHandler
    {        
        class SignalHandler
        {
            int			signal_;
            std::string		name_;
	    struct sigaction	old_action_;
                       
            static void action( int , siginfo_t *, void * );
            
	  public:

	    SignalHandler( int signal, const char * sig_name )
	      : signal_( signal ),
                name_( sig_name )
	    {
		struct sigaction sa;

		sa.sa_sigaction = action;
		sigemptyset ( &sa.sa_mask );
		sa.sa_flags = SA_SIGINFO;

		::sigaction( signal_, &sa, &old_action_ );
            }

	    ~SignalHandler( )
	    {
		::sigaction( signal_, &old_action_, 0 );
	    }
	};
                
      public:
        friend class SignalHandler;
        
        ErrorHandler();
        ~ErrorHandler();
        
      private:  
        
        static void recursion_preventer();
        static void abort( const ers::Issue & issue );
        static void terminate_handler();
        
        static std::map<int,SignalHandler*> handlers;
    };
    
    void ErrorHandler::SignalHandler::action( int signal, siginfo_t *, void * )
    {
	recursion_preventer();
        
        Configuration::instance().verbosity_level( 13 );
        ErrorHandler::abort( ers::SignalCatched( ERS_HERE, signal, handlers[signal]->name_.c_str() ) );
    }   
    
    ErrorHandler::ErrorHandler()
    {
        if ( !::getenv( "TDAQ_ERS_NO_SIGNAL_HANDLERS" ) )
        {
	    handlers[SIGSEGV] = new SignalHandler( SIGSEGV, "Segmentation fault (invalid memory reference)" );
	    handlers[SIGBUS]  = new SignalHandler( SIGBUS, "Bus error (bad memory access)" );
	    handlers[SIGILL]  = new SignalHandler( SIGILL, "Illegal Instruction" );
	    handlers[SIGFPE]  = new SignalHandler( SIGFPE, "Floating point exception" );
	    std::set_terminate( terminate_handler );
	    std::set_unexpected( terminate_handler );
        }
    }
    
    ErrorHandler::~ErrorHandler()
    {
        std::map<int,SignalHandler*>::iterator it;
        for( it = handlers.begin(); it != handlers.end(); ++it )
            delete it->second;
    }
    
    void ErrorHandler::recursion_preventer()
    {
	static bool same_shit_different_time = false;
	ERS_ASSERT( !same_shit_different_time )
	same_shit_different_time = true;
    }
    
    void ErrorHandler::terminate_handler()
    {
	recursion_preventer();
            
	Configuration::instance().verbosity_level( 13 );
    	try {
            throw;
        }
        catch( ers::Issue & ex ) {
            ErrorHandler::abort( UnhandledException( ERS_HERE, ex.get_class_name(), ex ) );
        }
        catch( std::exception & ex ) {
            ErrorHandler::abort( UnhandledException( ERS_HERE, "std::exception", ex ) );
        }
        catch(...) {
            ErrorHandler::abort( UnhandledException( ERS_HERE, "unknown" ) );
        }
    }
    
    void ErrorHandler::abort( const ers::Issue & issue )
    {
	StreamManager::instance().report_issue( Fatal, issue );
        ERS_ASSERT(0);
    }
}

std::map<int,ers::ErrorHandler::SignalHandler*> ers::ErrorHandler::handlers;

namespace
{
    ers::ErrorHandler __handler__;
}
