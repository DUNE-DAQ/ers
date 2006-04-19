/*
 *  OutputStream.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 02.12.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */


#ifndef ERS_OUTPUT_STREAM_H
#define ERS_OUTPUT_STREAM_H

#include <string>
#include <memory>
#include <ers/Issue.h>

/** \file OutputStream.h Defines abstract interface for ERS output streams.
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
    
    class OutputStream
    {
      friend class StreamManager;
      
      public:
	virtual ~OutputStream()
        { ; }
        
	virtual void write( const Issue & issue ) = 0;	/**< \brief Sends an issue into the stream */
      
      protected:
        OutputStream( );
                
	OutputStream & chained( );
        
      private:
	//
	// Disable copying
	//
	OutputStream( const OutputStream & other );
        OutputStream & operator=( const OutputStream & );
        
	void chained( OutputStream * stream );
        
      	std::auto_ptr<OutputStream> m_chained;
    };
}

#include <ers/internal/macro.h>

#endif

