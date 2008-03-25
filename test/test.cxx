/*
 *  test.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/ExampleIssues.h>
#include <ers/OutputStream.h>
#include <ers/StreamManager.h>

#include <ers/ers.h>
#include <stdexcept>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

void pass( int step )
{
    ERS_RANGE_CHECK( 0, step, 8 );
    
    ERS_DEBUG( 0, "performing step #" << step )
    switch ( step )
    {
    	case 1:
	    {
		ers::PermissionDenied issue( ERS_HERE, "foo2", 0x777 );
		throw issue;
	    }
        case 2:
	    {
		ers::FileDoesNotExist issue( ERS_HERE, "foo3" );
		throw issue;
	    }
        case 3:
	    {
		ers::FileDoesNotExist issue( ERS_HERE, "foo3" );
		issue.add_qualifier( "ers_test" );
		throw issue;
	    }
        case 4:
	    {
		for ( int level = 0; level < 4; level++ )
		    ERS_DEBUG( level, "Debug message with level " << level );
	    }
	    break;
        case 5:
	    {
		throw std::runtime_error( "std::out_of_range error" );
	    }
        case 6:
	    {
		ERS_ASSERT_MSG( step <= 6, "it is been checked that ERS_PRECONDITION works properly" );
	    }
	    break;
        case 7:
	    {
		ERS_ASSERT_MSG( step > 6, "it is been checked that ERS_PRECONDITION works properly" );
	    }
	    break;
        default:
	    {
		ERS_INFO( "Unhandled exception will be thrown now. This is intentional !!! Program will be aborted. " );
		struct UnhandledException {};
		throw UnhandledException();
	    }
    }
}

void test_function( int index )
{
    ERS_LOG( "starting thread #" << index )
    sleep( 1 );
    ers::error( ers::FileDoesNotExist( ERS_HERE, "error file" ) );
    sleep( 1 );
    ers::fatal( ers::FileDoesNotExist( ERS_HERE, "fatal file" ) );
    sleep( 1 );
    ers::warning( ers::FileDoesNotExist( ERS_HERE, "warning file" ) );
    ERS_LOG( "finishing thread #" << index )
}

struct IssueCatcher
{
    void handler( const ers::Issue & issue )
    {
    	std::cout << "IssueCatcher has been called for the following issue:" << std::endl;
        ers::error( issue );
    }
};

int main(int , char** )
{
    int step = 0;
       
    test_function( 0 );
    test_function( 0 );
    
    IssueCatcher catcher;
    try
    {
    	ers::set_issue_catcher( boost::bind( &IssueCatcher::handler, &catcher, _1 ) );
    }
    catch( ers::IssueCatcherAlreadySet & ex )
    {
    	ers::fatal( ex );
        return 1;
    }
    
    ERS_DEBUG( 0, "Testing output produced by different threads ... " );
    boost::thread thr1( boost::bind(test_function,1) );
    boost::thread thr2( boost::bind(test_function,2) );
    boost::thread thr3( boost::bind(test_function,3) );
    boost::thread thr4( boost::bind(test_function,4) );
    sleep( 4 );
    
    test_function( 0 );
    test_function( 0 );
    
    while( step++ < 10 )
    {
	try
	{
	    pass( step );
	}
	catch ( ers::PermissionDenied & ex )
	{
	    ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	    issue.add_qualifier( "q1" );
            ers::warning( issue );
	}
	catch ( ers::FileDoesNotExist & ex )
	{
	    ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	    issue.add_qualifier( "q2" );
	    ers::warning( issue );
	}
	catch ( ers::Issue & ex )
	{
	    ERS_DEBUG( 0, "Unknown issue caught: " << ex )
            ers::error( ex );
	}
	catch ( std::exception & ex )
	{
	    ers::CantOpenFile issue( ERS_HERE, "unknown", ex );
	    issue.add_qualifier( "q3" );
	    ers::warning( issue );
	}
    }
    return 0 ; 
}

