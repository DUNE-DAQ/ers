/*
 *  InvalidReferenceIssue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 04.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_INVALID_REFERENCE
#define ERS_INVALID_REFERENCE

#include "ers/Issue.h"

namespace ers {

    /** This issue represents an invalid reference. 
      * This class can be used for any type of reference, 
      * but most of the present mechanisms are designed for C/C++ memory references,
      * aka. pointers.
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief Invalid pointer issue. 
      */
    
    class InvalidReferenceIssue : public Issue {
protected:
	void pointer(const void *p); 
	InvalidReferenceIssue(const Context &context, ers_severity s) ; 
public:
	static const char* const CLASS_NAME ; 
	static const char* const REFERENCE_VALUE_KEY ;
	static const char* const REFERENCE_TYPE_KEY ; 
	static const char* const REFERENCE_NAME_KEY ; 
	static const char* const REFERENCE_C_PTR_TYPE ; 
	InvalidReferenceIssue() ;
	InvalidReferenceIssue(const Context &context, ers_severity s, const void* pointer, const std::string &message, ers_responsibility r = ers_resp_unknown) ; 
	virtual const char*get_class_name() const throw() ;
	static void check_reference(const Context &context, ers_severity s, const void* pointer, const char* ptr_name, ers_responsibility r); 
    } ; // InvalidReferenceIssue
    
} // ers

/** \def ERS_PRE_CHECK_PTR(p) checks that C/C++ pointer \c p is valid (non null) and throws an ers::InvalidReferenceIssue if not
  * If the pointer is invalid, this is considered as a client responsibility. 
  */

#ifndef N_DEBUG
#define ERS_PRE_CHECK_PTR(p) ers::InvalidReferenceIssue::check_reference(ERS_HERE,ers::ers_error,p,#p,ers::ers_resp_client)
#else 
#define ERS_PRE_CHECK_PTR(p) 
#endif

/** \def ERS_CHECK_PTR(p) checks that C/C++ pointer \c p is valid (non null) and throws an ers::InvalidReferenceIssue if not
 * If the pointer is invalid, this is considered as a server responsibility. 
 */

#ifndef N_DEBUG
#define ERS_CHECK_PTR(p) ers::InvalidReferenceIssue::check_reference(ERS_HERE,ers::ers_error,p,#p,ers::ers_resp_server)
#else 
#define ERS_CHECK_PTR(p) 
#endif

#endif

