/*
 *  RangeIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 15.02.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_RANGE_ISSUE
#define ERS_RANGE_ISSUE

#include "ers/InvalidReferenceIssue.h"

namespace ers {
    class RangeIssue : public InvalidReferenceIssue {
protected:
	RangeIssue(const ers::Context &c, ers::severity_t s) ; 
public:
	static void set_range(ers::Issue &issue, long index, long min_index, long max_index, const char* entity_name=0) ;
	static void check_range(const ers::Context &c, long min, long value, long max, const char* entity_name=0); 
	static const char * const CLASS_NAME ; 
	static const char * const INDEX_REFERENCE_TYPE ; 
	static const char * const RANGE_MIN_KEY ; 
	static const char * const RANGE_MAX_KEY ; 
	RangeIssue() ; 
	RangeIssue(const ers::Context &c, ers::severity_t s, long min_index, long index, long max_index, const char* entity_name=0) ; 
	virtual const char*get_class_name() const throw() ;

    } ; // RangeIssue
    
} ; // ers

#if (!defined(N_ERS_ASSERT))
#define ERS_RANGE_CHECK(min,value,max) ers::RangeIssue::check_range(ERS_HERE,min,value,max,#value)
#else 
#define ERS_RANGE_CHECK(min,value,max) 
#endif
#endif

