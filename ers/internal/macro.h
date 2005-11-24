#ifndef ERS_MACRO_H
#define ERS_MACRO_H

#include <ers/ers.h>
 
#define ERS_INTERNAL_DEBUG( level, message ) { \
if ( ers::debug_level() >= level ) \
{ \
    std::ostringstream out; \
    out << message; \
    ers::Info info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Severity( ers::Debug, level ) ); \
    std::cout << info << std::endl; \
} }
 
#define ERS_INTERNAL_WARNING( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::Info info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Warning ); \
    std::cerr << info << std::endl; \
}
 
#define ERS_INTERNAL_ERROR( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::Info info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Error ); \
    std::cerr << info << std::endl; \
}
 
#endif
