/*
 *  NotImplemented.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Assertion.h"

namespace ers {
    
    /** This special 'assertion' is used to mark a non implemented function or method. 
     *  Typically, this is marked using the NOT_IMPLEMENTED() macro. 
     *  @author Matthias Wiesmann
     *  @version 1.0
     */
    
    class NotImplemented : public Assertion {
	
protected:
	virtual std::string build_message(const char* condition_text, const std::string &message, bool constant_expression = false) ; 
public:
	static const char* NOTIMPLEMENTED_CLASS ; 
	NotImplemented();
	NotImplemented(const Context &context, ers_severity s) ;
	virtual const char*get_class_name() const throw();
    } ;  // Precondition
} // ers

#define NOT_IMPLEMENTED() throw NotImplemented(ERS_HERE,ers::ers_error)
#define NOT_IMPLEMENTED_FATAL() throw NotImplemented(ERS_HERE,ers::ers_fatal)

