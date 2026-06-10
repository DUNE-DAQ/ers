/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from ers/OutputStream.h to include/ers/OutputStream.hpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

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
#include <ers/Issue.hpp>

/** \file OutputStream.h Defines abstract interface for ERS output streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

namespace ers
{
    
    class Issue; 

    /** The abstract ERS output stream interface.
      * This interface defines the pure virtual method to \c write issues to the stream.
      * Any subclass must implement this method.
      *
      * \author Serguei Kolos
      * \version 1.0
      * \brief ERS abstract output stream interface.
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
	OutputStream( const OutputStream & other ) = delete;
        OutputStream & operator=( const OutputStream & ) = delete;
        
	void chained( OutputStream * stream );
                
      	std::unique_ptr<OutputStream> m_chained;
    };
}

#include <ers/internal/macro.hpp>

#endif

