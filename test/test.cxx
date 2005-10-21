/*
 *  test.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/ExampleIssues.h>
#include <ers/ers.h>
#include <stdexcept>

/** \file test.cxx 
  * This file contains a very simple example of using ERS. 
  * Basically, we use the custom issue defined in ExampleIssue. 
  */

void pass( int step )
{
    ERS_RANGE_CHECK( 0, step, 6 );
    
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
            struct UnhandledException {};
            throw UnhandledException();
        }
        case 7:
        {
            ERS_ASSERT_MSG( step <= 6, "it is been checked that ERS_PRECONDITION works properly" );
        }
        default:
        {
            ERS_ASSERT_MSG( step <= 6, "ERS_PRECONDITION does not work properly" );
        }
    }
}

int main(int , char** )
{
    int step = 0;
    while( step++ < 10 )
    {
	try
	{
	    pass( step );
	}
	catch ( ers::PermissionDenied & ex )
	{
	    ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
	    ers::warning( issue );
	}
	catch ( ers::FileDoesNotExist & ex )
	{
	    ers::CantOpenFile issue( ERS_HERE, ex.get_file_name(), ex );
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
	    ers::warning( issue );
	}
    }
    return 0 ; 
} // main 

