/*
 *  FilterStream.h
 *  ERS
 *
 *  Created by Matthias Wiesmann on 31.03.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_FILTERSTREAM 
#define ERS_FILTERSTREAM

#include "ers/Stream.h"

namespace ers {
    
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
      * <nobr><code>filter:filter:?include_qualifier1,include_qualifier2!exclude_qualifier1,exclude_qualifier2\@stream_identifier</code></nobr>
      * The stream_identifier can be any stream_key used by ERS (including a second filter). 
      * 
      * For more information on associating a stream to severity level, see the documentation on the StreamFactory class.
      * \see ers::StreamFactory
      * \author Matthias Wiesmann
      * \version 1.0
      */
    
    class FilterStream : public Stream {
protected:
	Stream *m_target_stream_ptr ;                                /**< \brief chained target stream */
	std::vector<std::string> m_include ;                         /**< \brief include list */
	std::vector<std::string> m_exclude ;                         /**< \brief exclude list */
	FilterStream(const FilterStream &other);                    
public:
	static const char *const FILTER_STREAM_TAG ; 
	static FilterStream *factory(const std::string &include_str, const::std::string &exclude_str, const std::string &target_str) ; 
	static FilterStream *factory(const std::string &parameters); 
	FilterStream(Stream *target_ptr, 
		     const std::vector<std::string> & include_list, 
		     const std::vector<std::string> & exclude_list) ; 
	~FilterStream() ;                                             /**< \brief destructor */
	virtual bool is_accept(const Issue* issue_ptr);               /**< \brief filter method */
	virtual void send(const Issue *issue_ptr) ;                   /**< \brief send method */
	
    } ; // FilterStream
    
} ; // ers

#endif
