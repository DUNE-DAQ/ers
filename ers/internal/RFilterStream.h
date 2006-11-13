/*
 *  RFilterStream.h
 *  ERS
 *
 *  Created by Andrea Negri on 13.11.06.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_STREAM_RFILTER_H 
#define ERS_STREAM_RFILTER_H

#include <ers/OutputStream.h>
#include <boost/regex.hpp>

namespace ers
{    
    /** This stream offers regular expression filtering capacities 
      * It basically hooks up in front of another stream and filters the data output.
      * Filtering is based on two list and \e include list and and \e exclude list
      * To be accepted an Issue needs two match two criteria 
      * \li No qualifier in the issue should match strings in the exclude list 
      * \li At least one qualifier in the issue should match a string in the include list
      *
      * \note: Contrary to the FilterStream, the RFilterStream before looking for matches
      *        in the qualifier list, it checks against the function full name
      *        ( ie: issue.context().function_name() )   
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
    
    class RFilterStream : public OutputStream
    {
      public:
	RFilterStream( const std::string & format ); 
	
        void write( const Issue & issue );		/**< \brief write method */
        
      private:	    
        bool is_accepted( const ers::Issue & issue );
        std::vector<boost::regex> m_regInclude;             /**< \brief include list */
        std::vector<boost::regex> m_regExclude;             /**< \brief exclude list */   
    };
}

#endif
