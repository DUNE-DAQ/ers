/*
 *  Stream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#ifndef ERS_STREAM_H
#define ERS_STREAM_H

#include <string>
#include <iostream>
#include <memory>

#include <ers/Issue.h>

/** \file Stream.h Defines abstract interface for ERS streams.
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
    
    class Stream
    {
      friend class StreamFactory;
      
      public:
	virtual ~Stream()
        { ; }
        
	virtual void write( const Issue & issue ) = 0;	/**< \brief Sends an issue into the stream */
	virtual Issue * read() = 0;			/**< \brief Receives an issue from the stream */
      
      protected:
        Stream( );
                
	Stream & chained( );
        
      private:
	//
	// Disable copying
	//
	Stream( const Stream & other );
        Stream & operator=( const Stream & );
        
	void chained( Stream * stream );
        
      	std::auto_ptr<Stream> m_chained;
    };
}

#include <ers/StreamFactory.h>

#define ERS_REGISTER_STREAM( class, name, param ) \
namespace { \
    struct StreamRegistrator { \
	static ers::Stream * create( const std::string & param ) \
	{ return new class( param ); }  \
        StreamRegistrator() \
	{ ers::StreamFactory::instance().register_stream( name, create ); } \
    } registrator; \
}

#endif

