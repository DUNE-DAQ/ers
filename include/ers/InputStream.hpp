/*
 *  InputStream.h
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_INPUT_STREAM_H
#define ERS_INPUT_STREAM_H

#include <string>
#include <ers/Issue.hpp>
#include <ers/IssueReceiver.hpp>

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
#define DEFINE_DUNE_ERS_INPUT_STREAM(klass)                                                                            \
  EXTERN_C_FUNC_DECLARE_START                                                                                          \
  ers::InputStream* make(const std::initializer_list<std::string> & params)                                            \
  {                                                                                                                    \
    return dynamic_cast<ers::InputStream*>(new klass(params));                                                         \
  }                                                                                                                    \
  }
  

/** \file InputStream.h Defines abstract interface for ERS input streams.
 * \author Serguei Kolos
 * \brief ers header and documentation file
 */
namespace ers {
    class Issue;

    /** ERS Issue input stream interface.
     *
     * \author Serguei Kolos
     * \brief ERS Issue input stream interface.
     */
    class InputStream {
        friend class StreamManager;

    public:
        virtual ~InputStream() {
            ;
        }

    protected:
        InputStream();

        /**< \brief Will be called when a new issue is received */
        void receive(const Issue &issue);

    private:
        InputStream(const InputStream &other) = delete;
        InputStream& operator=(const InputStream&) = delete;

        void set_receiver(IssueReceiver *receiver) {
            m_receiver = receiver;
        }

        IssueReceiver *m_receiver;
    };
}

#include <ers/StreamFactory.hpp>

#define ERS_REGISTER_INPUT_STREAM( class, name, params ) \
namespace { \
    struct InputStreamRegistrator { \
	static ers::InputStream * create( const std::initializer_list<std::string> & params ) \
	{ return new class( params ); }  \
        InputStreamRegistrator() \
	{ ers::StreamFactory::instance().register_in_stream( name, create ); } \
    } registrator_mp; \
}

#endif

