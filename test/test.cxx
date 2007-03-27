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

struct MyStream : public ers::OutputStream
{
    void write( const ers::Issue & ex )
    {
    	std::cout << "------------------------------- My ad hoc stream -------------------------------" << std::endl;
    	std::cout << ex << std::endl;
    	std::cout << "--------------------------------------------------------------------------------" << std::endl;
    }
};

template <class ClassParameterType1, class ClassParameterType2>
struct Test
{
    template <class FunctionParameterType1, class FunctionParameterType2>
    static void
    function( FunctionParameterType1 , FunctionParameterType2 )
    {
    	ERS_INFO( "test the details level of the issue location printout" );
    }
};  
  
void test_function( int index )
{
    for( int i = 0; i < 10; i++ )
    {
	ERS_DEBUG( 0, "performing step # " << i << " in thread " << index )
	ERS_INFO( "info #" << i << " in thread " << index )
	usleep( 1 );
    }
}

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

int main(int , char** )
{
    int step = 0;
    
    ers::StreamManager::instance().add_output_stream( ers::Warning, new MyStream );
    
    ERS_DEBUG( 0, "Testing output produced by different threads ... " );
    boost::thread thr1( boost::bind(test_function,1) );
    boost::thread thr2( boost::bind(test_function,2) );
    boost::thread thr3( boost::bind(test_function,3) );
    boost::thread thr4( boost::bind(test_function,4) );
    sleep( 2 );
    
    Test<int,float>::function( 1, 2 );
    Test<int,float>::function( 1.1, 2.2 );
    
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

