#include <iostream>
#include <fcntl.h>
#include <stdlib.h>

#include <xercesc/util/PlatformUtils.hpp>

#include "SystemTest.h"

#include "ers/TabOutStream.h"
#include "ers/TabInStream.h"
#include "ers/XercesStream.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/StreamFactory.h"
#include "system/SignalIssue.h"

#include "system/OpenFail.h"

#include "ExampleIssue.h"
#include "SystemTest.h"

using namespace ers ; 

void test_write(const Issue &e, const char* path) {
    TabOutStream tab_out(path);
    tab_out << e ; 
} // test_write

void test_read(const char* path) {
    TabInStream tab_in(path) ; 
    Issue *i = tab_in.receive() ; 
    ERS_CHECK_PTR(i);
    throw *i ; 
} // test_read

void test_write2(const Issue &e, const char* path) {
    // TinyXMLStream xml_out(path);
    XercesStream xml_out(path); 
    xml_out << e ; 
} // test_write

void test_read2(const char* path) {
    // TinyXMLStream xml_in(path) ; 
    XercesStream xml_in(path); 
    Issue *i = xml_in.receive() ; 
    ERS_CHECK_PTR(i);
    throw *i ; 
} // test_read

void test_issue(const Issue &e) {
    try {
	const char* path = "/tmp/out.xml" ; 
	test_write2(e,path);
	test_read2(path); 
    } catch (Issue &i) {
	// XMLStream xml("/tmp/out.xml");
	// xml << i ; 
	StreamFactory::error(&i); 
    } // try / catch 
} // test_issue

int main(int argc, char* argv[]) {
    XMLPlatformUtils::Initialize(); 
    try {
	throw EXAMPLE_ERROR(5); 
    } catch (Issue &e) {
	 test_issue(e); 
    }
    return 0 ; 
} // main 

