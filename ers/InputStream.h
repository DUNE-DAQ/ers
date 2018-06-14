/*
 *  InputStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#ifndef ERS_INPUT_STREAM_H
#define ERS_INPUT_STREAM_H

#include <string>
#include <ers/Issue.h>
#include <ers/IssueReceiver.h>

/** \file InputStream.h Defines abstract interface for ERS input streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

namespace ers
{
    
    class Issue; 

    /** ERS Issue input stream interface.
      * An ERS input stream is a mean to receive issues.
      * The core method to do so is called \c receive.
      * Any subclass of input stream must implement this method.
      * \author Serguei Kolos
      * \version 1.0
      * \brief ERS Issue stream interface.
      */
    
    class InputStream
    {
      friend class StreamManager;
      
      public:
	virtual ~InputStream()
        { ; }
              
      protected:
        InputStream( );
                        
        /**< \brief Will be called when the new issue is received */
	void receive( const Issue & issue );
      
      private:
	//
	// Disable copying
	//
	InputStream( const InputStream & other ) = delete;
        InputStream & operator=( const InputStream & ) = delete;
        
        void set_receiver( IssueReceiver * receiver )
        { m_receiver = receiver; }
        
        IssueReceiver * m_receiver;
    };
}

#include <ers/StreamFactory.h>

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

