/*
 *  Precondition.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include "ers/Assertion.h"

#ifndef ERS_PRECONDITION_
#define ERS_PRECONDITION_

namespace ers {
    
    /** A precondition is a special type of assertion that checks the entry condition for a function.
     * Typically, precondition check entry values for function, and members state in object methods
     * \author Matthias Wiesmann
     * \version 1.0 
     * \brief Precondition issue. 
     */
    
    class Precondition : public Assertion {
protected:
	virtual std::string build_message(const char* condition_text, const std::string &message, bool constant_expression = false) ; 
	Precondition(const Context &context, ers_severity s); 
public:
	static const char* PRECONDITION_CLASS ; 
	Precondition();
	Precondition(const Context &context, ers_severity s, const char*condition, const std::string &message , bool constant_expression=false) ;
	virtual const char*get_class_name() const throw();
    } ;  // Precondition
} // ers


/** \def ERS_PRECONDITION(expr,msg,params) This macro inserts an assertion than checks condition e, 
 * if e is not true, then an issue of type ers::Asertion is thrown with message m. 
 * If the compiler is gcc, then the transient field of the assertion is set according to the transcience of the expression. 
 * This means that if the expression is detected by the compiler as being constant. 
 * If the macro \c N_DEBUG is defined then the macros are disabled. 
*/

#ifndef N_DEBUG
#ifdef __GNUC__
#define ERS_PRECONDITION(expr,...) { if(!(expr)) { char precondition_buffer[256] ; snprintf(precondition_buffer,256, __VA_ARGS__) ; ers::Precondition failed_precondition(ERS_HERE,ers::ers_error,#expr,precondition_buffer,__builtin_constant_p(expr)) ; throw failed_precondition ; } }
#else 
#define ERS_PRECONDITION(expr,...) { if(!(expr)) { char precondition_buffer[256] ; snprintf(precondition_buffer,256, __VA_ARGS__) ; ers::Precondition failed_precondition(ERS_HERE,ers::ers_error,#expr,precondition_buffer,false) ; throw failed_precondition ; } }
#endif
#else 
#define ERS_PRECONDITION(expr,...) 
#endif

#endif

