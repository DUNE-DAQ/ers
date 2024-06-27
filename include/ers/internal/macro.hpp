#ifndef ERS_MACRO_H
#define ERS_MACRO_H

/** \file macro.h This file defines some internal ERS macros.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#include <ers/ers.hpp>
#include <ers/StreamFactory.hpp>
#include <ers/StandardStreamOutput.hpp>
#include <boost/preprocessor/cat.hpp>

#include "cetlib/BasicPluginFactory.h"
#include "cetlib/compiler_macros.h"

#ifndef EXTERN_C_FUNC_DECLARE_START
// NOLINTNEXTLINE(build/define_used)
#define EXTERN_C_FUNC_DECLARE_START                                                                                    \
  extern "C"                                                                                                           \
  {
#endif

/**
 * @brief Declare the function that will be called by the plugin loader
 * @param klass Class to be defined as a DUNE DAQ Module
 */
// NOLINTNEXTLINE(build/define_used)
#define DEFINE_DUNE_ERS_OUTPUT_STREAM(klass, param)                                                                    \
  EXTERN_C_FUNC_DECLARE_START                                                                                          \
  ers::OutputStream* make(const std::string & param)                                                                                            \
  {                                                                                                                    \
    return dynamic_cast<ers::OutputStream*>(new klass(param));                                                         \
  }                                                                                                                    \
  }

ERS_DECLARE_ISSUE( ers, InternalMessage, ERS_EMPTY, ERS_EMPTY )

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
    ers::InternalMessage info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Severity( ers::Debug, level ) ); \
    ers::StandardStreamOutput::println( std::cout, info, 0 ); \
} }

#define ERS_INTERNAL_INFO( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::InternalMessage info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Information ); \
    ers::StandardStreamOutput::println( std::cout, info, 0 ); \
}
 
#define ERS_INTERNAL_WARNING( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::InternalMessage info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Warning ); \
    ers::StandardStreamOutput::println( std::cerr, info, 0 ); \
}
 
#define ERS_INTERNAL_ERROR( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::InternalMessage info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Error ); \
    ers::StandardStreamOutput::println( std::cerr, info, 0 ); \
}
 
#define ERS_INTERNAL_FATAL( message ) { \
    std::ostringstream out; \
    out << message; \
    ers::InternalMessage info( ERS_HERE, out.str() ); \
    info.set_severity( ers::Fatal ); \
    ers::StandardStreamOutput::println( std::cerr, info, 0 ); \
    ::exit( 13 ); \
}
 
#endif
