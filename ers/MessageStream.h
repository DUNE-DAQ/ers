/*
 *  MessageStream.h
 *  Test
 *
 *  Created by Matthias Wiesmann on 25.02.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */


#include "ers/STLStream.h"

namespace ers {
    
    /** This stream simply displays the message from one issue. 
      * It is meant to be used with log messages
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief simple message stream
      */
    
    class MessageStream : public STLStream {
public:
	static const char* const SUFFIX ;           /**< \brief suffix used to identify this stream / file */
	MessageStream() ;                           /**< \brief empty constructor default to stderr */
	MessageStream(std::ostream *out_stream) ;   /**< \brief constructor with output stream */
	MessageStream(const System::File &file) ;   /**< \brief constructor with file (output) */
	~MessageStream() ;                          /**< \brief destructor */
	virtual void send(const Issue *issue_ptr) ; /**< \brief sends the issue, displays the message */
    } ; // MessageStream
} ; // ers