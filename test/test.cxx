#include <iostream>
#include <fcntl.h>
#include <stdlib.h>

#include "ers/ers.h"
#include "ers/TabStream.h"

#include "ExampleIssue.h"

using namespace ers ; 



void foo(int n) {
    ERS_RANGE_CHECK(0,n,42) ; 
}

int main(int argc, char* argv[]) {
    // std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
    ers::Context::add_qualifier("ers_test") ;
    ers::IssueFactory::print_registered(); 
    ers::StreamFactory::print_registered(); 
    ERS_DEBUG_1("loading Xerxes");  
    ERS_DEBUG_1("done");
    try {
        ERS_DEBUG_1("checking static assert");
	ERS_STATIC_ASSERT(sizeof(int)==4); 
	ERS_DEBUG_1("throwing custom issue");
	foo(43); 
	throw EXAMPLE_ERROR(5); 
    } catch (Issue &e) {
	StreamFactory::warning(&e); 
    }
    return 0 ; 
} // main 

