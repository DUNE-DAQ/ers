#include <iostream>
#include <fcntl.h>
#include <stdlib.h>

#include <xercesc/util/PlatformUtils.hpp>

#include "ers/TabStream.h"
#include "ers/XercesStream.h"
#include "ers/ers.h"
#include "system/System.h"

#include "ExampleIssue.h"

using namespace ers ; 

void test_write1(const Issue &e, const char* path) {
    TabStream tab_out(path,false);
    tab_out << e ; 
} // test_write

void test_read1(const char* path) {
    System::File file(path); 
    TabStream tab_in(file,true) ; 
    Issue *i = tab_in.receive() ; 
    ERS_CHECK_PTR(i);
    throw *i ; 
} // test_read

void test_write2(const Issue &e, const char* path) {
    ERS_DEBUG_1("writing to %s",path);
    System::File file(path); 
    XercesStream xml_out(file,false); 
    xml_out << e ; 
    xml_out << ERS_LOG_ISSUE(ers::ers_debug_0,"Test"); 
} // test_write

void test_read2(const char* path) {
    ERS_DEBUG_1("Reading from %s",path);
    System::File file(path); 
    XercesStream xml_in(file,true); 
    Issue *i = xml_in.receive() ; 
    ERS_CHECK_PTR(i);
    ERS_DEBUG_1("Got Issue <<%s>>",i->message().c_str());
   
    throw *i ; 
} // test_read

void test_issue(const Issue &e) {
    try {
    	const char* path1 = "/tmp/out.tab" ; 
	test_write1(e,path1);
	// test_read1(path1); 
	const char* path2 = "/tmp/out.xml" ; 
	test_write2(e,path2);
	test_read2(path2); 
    } catch (Issue &i) {
	// XMLStream xml("/tmp/out.xml");
	// xml << i ; 
	printf("-----------------------------------\n"); 
	StreamFactory::warning(&i); 
    } // try / catch 
} // test_issue

void foo(int n) {
    ERS_RANGE_CHECK(0,n,42) ; 
}

int main(int argc, char* argv[]) {
    // std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
    ers::IssueFactory::print_registered(); 
    ers::StreamFactory::print_registered(); 
    ERS_DEBUG_1("loading Xerxes"); 
    XMLPlatformUtils::Initialize(); 
    ERS_DEBUG_1("done");
    try {
        ERS_DEBUG_1("checking static assert");
	ERS_STATIC_ASSERT(sizeof(int)==4); 
	ERS_DEBUG_1("throwing custom issue");
	foo(43); 
	throw EXAMPLE_ERROR(5); 
    } catch (Issue &e) {
	test_issue(e); 
    }
    return 0 ; 
} // main 

