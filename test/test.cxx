#include <iostream>
#include <fcntl.h>

#include "ers/OpenFail.h"
#include "ers/XMLOutStream.h"
#include "ers/TabOutStream.h"
#include "ers/TabInStream.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"
#include "ers/InvalidReferenceIssue.h"
#include "ers/File.h"

using namespace ers ; 

void test_write(const Issue &e, const char* path) {
    TabOutStream tab_out(path);
    tab_out << e ; 
} // test_write

void test_read(const char* path) {
    TabInStream tab_in(path) ; 
    Issue *i = tab_in.receive() ; 
    throw *i ; 
} // test_read

void test_issue(const Issue &e) {
    try {
	test_write(e,"/tmp/out.tab");
	test_read("/tmp/out.tab"); 
    } catch (Issue &i) {
	Stream::error(&i); 
    }
} // test_issue

int main(int argc, char* argv[]) {
    try {
	// NOT_IMPLEMENTED(); 
	// int fd = open("/etc/forbidden",O_RDWR |  O_CREAT,0); 
	// if (fd<0) throw OpenFail("/etc/forbidden",O_RDWR |  O_CREAT,ERS_HERE); 
	// XMLOutStream xml_crash("/etc/forbidden"); 
	ers::File f("/etc/passwd"); 
	f.permissions(S_IRWXU); 
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
