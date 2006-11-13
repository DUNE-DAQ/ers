/*
 *  FilterStream.cxx
 *  ers
 *
 *  Created by Matthias Wiesmann on 31.03.05.
 *  Modified by Serguei Kolos on 11.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/internal/FilterStream.h>
#include <ers/internal/Util.h>
#include <ers/StreamFactory.h>
#include <algorithm>

ERS_REGISTER_OUTPUT_STREAM( ers::FilterStream, "filter", format )

namespace
{
    const char NOT = '!';
    const char * const SEPARATORS = ";" ;
} // anonymous namespace 

/** Constructor 
  * \param chained the chained stream, which will be filtered. Only messages, which pass the filter
  * will go to the chained stream
  * on the stack and owned by the current object, i.e it will be deleted upon destruction
  * \param format describes filter expression.
  */
ers::FilterStream::FilterStream( const std::string & format )
{
    std::vector<std::string> tokens;
    ers::tokenize( format, SEPARATORS, tokens );
    for( size_t i = 0; i < tokens.size(); i++ )
    {
    	if ( !tokens[i].empty() && tokens[i][0] == NOT )
            m_exclude.push_back( tokens[i].substr( 1 ) );
        else
            m_include.push_back( tokens[i] );
    }
}


/** Filtering method 
  * This method checks if an Issue is to be accepted or not. 
  * \param issue_ptr the issue to check 
  * \return \c true if the Issue passes filtering, \c false otherwise.
  */
bool
ers::FilterStream::is_accepted( const ers::Issue & issue )
{
    const std::vector<std::string> & qualifiers = issue.qualifiers( );
               
    std::vector<std::string>::const_iterator it;
    for( it = m_exclude.begin(); it != m_exclude.end(); ++it )
    {
	std::vector<std::string>::const_iterator conflict = std::find( qualifiers.begin(), qualifiers.end(), *it );
	if ( conflict != qualifiers.end() )
	{
	    return false;
	}
    }
    
    for( it = m_include.begin(); it != m_include.end(); ++it )
    {
	std::vector<std::string>::const_iterator match = std::find( qualifiers.begin(), qualifiers.end(), *it );
	if ( match != qualifiers.end() )
	{
            return true;
	}
    }

    return m_include.empty(); 
}

/** Write method 
  * basically calls \c is_accept to check if the issue is accepted. 
  * If this is the case, the \c write method on the chained stream is called with 
  * \c issue. 
  * \param issue issue to be sent.
  */
void
ers::FilterStream::write( const ers::Issue & issue )
{
    if ( is_accepted( issue ) )
    {
	chained().write( issue ); 
    }
} // send

