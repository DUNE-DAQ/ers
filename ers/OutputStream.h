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
      * This interface defines two core methods to \c write issues and to \c configure the stream.
      * Certain subclasses of stream might implement only sending while leaving the configure method empty.
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
        
	virtual void write( const Issue & issue ) = 0;	/**< \brief Sends an issue into the stream */
	
        virtual void configure( const std::map<std::string,std::string> & config ) throw ();	/**< \brief Configures the stream */
      
      protected:
        OutputStream( );
                
	OutputStream & chained( );
        
        virtual bool isNull() const;
        
      private:
	//
	// Disable copying
	//
	OutputStream( const OutputStream & other );
        OutputStream & operator=( const OutputStream & );
        
	void chained( OutputStream * stream );
        
        void configure_stream_chain( const std::map<std::string,std::string> & config ) throw ();
        
      	std::auto_ptr<OutputStream> m_chained;
    };
}

#include <ers/internal/macro.h>

#endif

