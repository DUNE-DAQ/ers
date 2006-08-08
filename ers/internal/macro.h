#ifndef ERS_MACRO_H
#define ERS_MACRO_H

#include <ers/ers.h>
#include <ers/StreamFactory.h>
#include <boost/preprocessor/cat.hpp>

#define ERS_REGISTER_OUTPUT_STREAM( class, name, param ) \
namespace { \
    struct BOOST_PP_CAT( OutputStreamRegistrator, __LINE__ ) { \
        static ers::OutputStream * create( const std::string & param ) \
        { return new class( param ); }  \
        BOOST_PP_CAT( OutputStreamRegistrator, __LINE__ ) ()\
        { ers::StreamFactory::instance().register_out_stream( name, create ); } \
    } BOOST_PP_CAT( registrator, __LINE__ ); \
}
 
#define ERS_INTERNAL_DEBUG( level, message ) { \
if ( ers::debug_level() >= level ) \
{ \
    std::ostringstream out; \
    out << message; \
    ers::DEBUG info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Severity( ers::Debug, level ) ); \
    std::cout << info << std::endl; \
} }

#define ERS_INTERNAL_INFO( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::INFO info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Information ); \
    std::cerr << info << std::endl; \
}
 
#define ERS_INTERNAL_WARNING( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::WARNING info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Warning ); \
    std::cerr << info << std::endl; \
}
 
#define ERS_INTERNAL_ERROR( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::ERROR info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Error ); \
    std::cerr << info << std::endl; \
}
 
#endif
