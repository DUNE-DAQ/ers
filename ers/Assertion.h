/*
 *  Assertion.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */



#ifndef ERS_ASSERTION
#define ERS_ASSERTION

#include <stdio.h>
#include "ers/Issue.h"

namespace ers {
    
/** This class represents an assertion in the code, it is an issue with two fields: 
  * \li A condition 
  * \li A message describing the condition 
  *
  * If the condition is not verified, an Issue is trown. 
  * \author Matthias Wiesmann
  * \version 0.1 
  * \brief This Issue represents a basic assertion
  */
    
    class Assertion: public ers::Issue {
protected:
    virtual std::string build_message(const char* condition_text, const std::string &msg, bool constant_expression=false) ; 
    Assertion(ers_severity s, const Context &context); 
    void setup(const char *condition, const std::string &message, bool constant_expression); 
public:
    static const char* ASSERT_CONDITION_KEY ;
    static const char *ASSERTION_CLASS_NAME ; 
    virtual const char*get_class_name() const throw() ;
    Assertion(); 
    Assertion(const char*condition, const std::string &message, ers_severity s, const Context &context, bool constant_expression=false) ;
} ; 

template <bool> struct Compile_time_error ; 
template<> struct Compile_time_error<true> { };

} // ers 

/** \def ERS_STATIC_ASSERT(expr) This macro inserts a compile-time assertion into the code. 
 * Compile time assertion can only be done on constant factors (i.e compile-time known quantities).  
 *
 */

#ifndef N_STATIC_ASSERT
#define ERS_STATIC_ASSERT(expr) { ers::Compile_time_error <((expr) != 0)> ERROR_ASSERTION_FAILED ;   (void) ERROR_ASSERTION_FAILED ; }
#else 
#define ERS_STATIC_ASSERT(expr) ;
#endif


/** \def ERS_ASSERT(expr,msg,...) This macro inserts an assertion than checks condition e, 
 * if e is not true, then an issue of type ers::Asertion is thrown with message msg. 
 * The msg is actually a formatting string, like printf, that can be used with subsequent parameters. 
 * If the compiler is gcc, then the transient field of the assertion is set according to the transcience of the expression. 
 * This means that if the expression is detected by the compiler as being constant, 
 */

#ifdef __GNUC__
#define ERS_ASSERT(expr,...) { if(!expr) { char assertion_buffer[256] ; snprintf(assertion_buffer,sizeof(assertion_buffer), __VA_ARGS__) ; ers::Assertion failed_assertion(#expr,assertion_buffer,ers::ers_error,ERS_HERE,__builtin_constant_p(expr)) ; throw failed_assertion ; } }
#else 
#define ERS_ASSERT(expr,...) { if(!expr) { char assertion_buffer[256] ; snprintf(assertion_buffer,sizeof(assertion_buffer), __VA_ARGS__) ; ers::Assertion failed_assertion(#expr,assertion_buffer,ers::ers_error,ERS_HERE,false) ; throw failed_assertion ; } }
#endif

#endif

