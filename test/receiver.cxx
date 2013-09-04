/*
 *  receiver.cxx
 *  Test for issue receiver
 *
 *  Created by Serguei Kolos on 24.11.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/InputStream.h>
#include <ers/ExampleIssues.h>
#include <ers/ers.h>
#include <semaphore.h>

struct MyIssueReceiver : public ers::IssueReceiver
{
    void receive( const ers::Issue & issue )
    {
    	std::cout << issue << std::endl;
    }
};

int main(int , char** )
{
    
    try {
    	ers::StreamManager::instance().add_receiver( "mts", "*", new MyIssueReceiver );
    }
    catch( ers::Issue & ex ) {
    	ers::fatal( ex );
    	return 1;
    }
    
    sem_t semaphore;
    sem_init( &semaphore, 0, 0 );
    sem_wait( &semaphore );
    
    return 0; 
}

