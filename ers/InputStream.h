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

    /** ERS Issue stream interface.
      * An ERS stream is a mean to send and receive issues.
      * The two core method to do so are \c write and \c read.
      * Certain subclasses of stream might implement only sending, or only receiving.
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
        
	void receive( const Issue & issue );	/**< \brief Must be called when the new issue is received */
      
      protected:
        InputStream( );
                        
      private:
	//
	// Disable copying
	//
	InputStream( const InputStream & other );
        InputStream & operator=( const InputStream & );
        
        void set_receiver( IssueReceiver * rec )
        { m_receiver = rec; }
        
        IssueReceiver * m_receiver;
    };
}

#include <ers/StreamFactory.h>

#define ERS_REGISTER_INPUT_STREAM( class, name, param ) \
namespace { \
    struct InputStreamRegistrator { \
	static ers::InputStream * create( ers::severity severity, const std::string & param ) \
	{ return new class( severity, param ); }  \
        InputStreamRegistrator() \
	{ ers::StreamFactory::instance().register_in_stream( name, create ); } \
    } registrator; \
}

#endif

