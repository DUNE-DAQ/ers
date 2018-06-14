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

    /** ERS Issue output stream interface.
      * An ERS output stream is a mean to send issues.
      * This interface defines the core method to \c write issues to the stream.
      * Any subclass of output stream must implement this method.
      * \author Serguei Kolos
      * \version 1.0
      * \brief ERS Output Issue stream interface.
      */
    
    class OutputStream
    {
      friend class StreamManager;
      
      public:
	virtual ~OutputStream()
        { ; }
        
	/**< \brief Sends the issue into this stream */
	virtual void write( const Issue & issue ) = 0;
	
      protected:
        OutputStream( );
                
	OutputStream & chained( );
        
        virtual bool isNull() const;
        
      private:
	//
	// Disable copying
	//
	OutputStream( const OutputStream & other ) = delete;
        OutputStream & operator=( const OutputStream & ) = delete;
        
	void chained( OutputStream * stream );
                
      	std::unique_ptr<OutputStream> m_chained;
    };
}

#include <ers/internal/macro.h>

#endif

