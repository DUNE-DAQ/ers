#include <iostream>
#include <fcntl.h>

#include <xercesc/util/PlatformUtils.hpp>


#include "ers/TabOutStream.h"
#include "ers/TabInStream.h"
#include "ers/TinyXMLStream.h"
#include "ers/XercesStream.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"
#include "ers/InvalidReferenceIssue.h"

#include "system/OpenFail.h"

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
	Stream::error(&i); 
    }
} // test_issue

int main(int argc, char* argv[]) {
    XMLPlatformUtils::Initialize(); 
    try {
	int fd = open("/etc/forbidden",O_RDWR |  O_CREAT,0); 
	if (fd<0) throw OpenFail(ERS_HERE,O_RDWR |  O_CREAT, "/etc/forbidden"); 
    } catch (Issue &e) {
	test_issue(e); 
    }
} // main 

// Stream::error(&e);

/* 
*/
// ERS_ASSERT(n,"foo");
//  ERS_PRECONDITION(3<2,"maths work %d %s",3<2,"argument");


/*  
if (fd<0) {
    
    issue_stream::error(&i);
} // if
 */
/*
 
 printf("starting\n");	
	ERS_DEBUG_0("creating file object");
	int s = 4096 ;
	MapFile f("/Users/wiesmann/.login",s,0,true,false); 
	ERS_DEBUG_0("trying map"); 
	f.map();
	void *a = f.address() ;
	printf("%p\n",a);
	char *p = (char *) calloc(sizeof(char),1024); 
	memcpy(p,a,1024);
	p[64] = 0 ; 
	printf("%s\n",p); 
 
 
 // NOT_IMPLEMENTED(); 
	// int fd = open("/etc/forbidden",O_RDWR |  O_CREAT,0); 
	// if (fd<0) throw OpenFail("/etc/forbidden",O_RDWR |  O_CREAT,ERS_HERE); 
	// XMLOutStream xml_crash("/etc/forbidden"); 
	// size_t big = 0x100000000000 ; 
	// void *p = malloc(big); 
	// ERS_ALLOC_CHECK(p,big); 
	// char  command[] = "/bin/echo" ; 
	// char param[] = "Hello World" ; 
	// char *array[] = { command, param, 0 } ; 
	// execv(command,array);
	// ers::Executable f("/bin/echo"); 
	// string_vector params ; 
	// params.push_back("hello world"); 
	// f.start(params); 
 
#include "ers/InvalidReferenceIssue.h"
#include "ers/Executable.h"
#include "ers/AllocIssue.h"
#include "ers/MapFile.h"
#include "ers/Stream.h"
 
 
 */
