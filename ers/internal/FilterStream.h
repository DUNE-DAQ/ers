/*
 *  FilterStream.h
 *  ERS
 *
 *  Created by Matthias Wiesmann on 31.03.05.
 *  Modified by Serguei Kolos on 11.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_STREAM_FILTER_H 
#define ERS_STREAM_FILTER_H

#include <ers/Stream.h>

namespace ers
{    
    /** This stream offers basic filtering capacities 
      * It basically hooks up in front of another stream and filters the data output.
      * Filtering is based on two list and \e include list and and \e exclude list
      * To be accepted an Issue needs two match two criteria 
      * \li No qualifier in the issue should match strings in the exclude list 
      * \li At least one qualifier in the issue should match a string in the include list
      * 
      * If the include list is empty, the second condition is not applied. 
      * This stream should only be used for severity levels where filtering makes sense, 
      * i.e warning, debugs, etc...
      * The syntax to request a filter stream is the following:<br> 
      * <code>filter:filter:?include_qualifier1,include_qualifier2!exclude_qualifier1,exclude_qualifier2\@stream_identifier</code>
      * The stream_identifier can be any stream_key used by the ERS system (including a second filter). 
      * 
      * For more information on associating a stream to a severity level, see the documentation on the StreamFactory class.
      * \see ers::StreamFactory
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief filtering stream 
      */
    
    class FilterStream : public Stream
    {
      public:
	FilterStream( const std::string & format ); 
	
        void write( const Issue & issue );		/**< \brief write method */
        
	Issue * read( )
        { return 0; } 

      private:	
        
        bool is_accepted( const ers::Issue & issue );
        
	std::vector<std::string> m_include;				/**< \brief include list */
	std::vector<std::string> m_exclude;				/**< \brief exclude list */
    };
}

#endif
