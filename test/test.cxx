#include <iostream>
#include <fcntl.h>

#include "ers/OpenFail.h"
#include "ers/XMLOutStream.h"
#include "ers/Precondition.h"
#include "ers/NotImplemented.h"

using namespace ers ; 

int main(int argc, char* argv[]) {
    try {
	NOT_IMPLEMENTED(); 
	int fd = open("/etc/forbidden",O_RDWR |  O_CREAT,0); 
	if (fd<0) throw OpenFail("/etc/forbidden",O_RDWR |  O_CREAT,ERS_HERE); 
    } catch (Issue &e) {
        Stream::error(&e);
	XMLOutStream xml("/tmp/out.xml");
	xml << e ; 
    }
} // main 


/* 
*/
// ERS_ASSERT(n,"foo");
//  ERS_PRECONDITION(3<2,"maths work %d %s",3<2,"argument");


/*  
if (fd<0) {
    
    issue_stream::error(&i);
} // if
 */
