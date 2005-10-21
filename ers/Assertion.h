/*
 *  Assertion.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 26.11.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

/** \file Assertion.h
  * This file defines the assertion class and the associated macros. 
  */

#ifndef ERS_ASSERTION_H
#define ERS_ASSERTION_H

#include <ers/Issue.h>
#include <ers/Severity.h>

/** This class represents an assertion in the code, it is an issue with two fields:
  * \li A condition
  * \li A reason
  *
  * If the condition is not verified, an Issue is trown.
  * \author Serguei Kolos
  * \version 1.0
  */

ERS_DECLARE_ISSUE(	ers,								// namespace
			Assertion,							// issue name
			"Assertion (" << condition << ") failed because " << reason,	// message
			((const char *)condition )					// first attribute
			((const char *)reason ) )					// second attribute

#ifndef ERS_NO_DEBUG
#include <boost/preprocessor/comparison.hpp>

/*\hideinitializer
*/
#define ERS_ASSERT_MSG( expression, message ) { \
if( !(expression) ) \
{ \
    std::ostringstream out; \
    out << BOOST_PP_IF( BOOST_PP_IS_EMPTY( message ), "of unknown reason", message ); \
    std::string reason = out.str(); \
    ers::Assertion ass( ERS_HERE, #expression, reason.c_str() ); \
    ers::fatal( ass ); \
}}
#else 
#define ERS_ASSERT_MSG( expression, message )
#endif

/** \def ERS_ASSERT(expression) This macro inserts an assertion than checks condition expression.
 * If expression is not true, then an issue of type ers::Asertion is thrown. It should be used
 * to check the internal attributes. 
 * \note This macro is disabled if the \c ERS_NO_DEBUG macro is defined
 */
#define ERS_ASSERT( expression )	ERS_ASSERT_MSG( expression, "of bad internal state" )

/** \def ERS_PRECONDITION(expression) This macro inserts an assertion than checks condition expression. 
 * If expression is not true, then an issue of type ers::Asertion is thrown. It should be used
 * to check the parameters, which are passed to functions. 
 * \note This macro is disabled if the \c ERS_NO_DEBUG macro is defined 
 */
#define ERS_PRECONDITION( expression )	ERS_ASSERT_MSG( expression, "of bad external parameter" )

/** \def ERS_RANGE_CHECK( min, val, max ) This macro inserts an assertion than checks 
 * the following condition \code ( min <= val ) && ( val <= max ) \endcode
 * If this expression is not true, then an issue of type ers::Asertion is thrown.
 * \note This macro is disabled if the \c ERS_NO_DEBUG macro is defined 
 */
#define ERS_RANGE_CHECK( min, val, max )		\
	ERS_ASSERT_MSG( (min) <= (val) && (val) <= (max), \
        			val << " is not in [" << min << "," << max << "] range" )
        
/** \def ERS_STRICT_RANGE_CHECK( min, val, max ) This macro inserts an assertion than checks 
 * the following condition \code ( min < val ) && ( val < max ) \endcode
 * If this expression is not true, then an issue of type ers::Asertion is thrown.
 * \note This macro is disabled if the \c ERS_NO_DEBUG macro is defined 
 */
#define ERS_STRICT_RANGE_CHECK( min, val, max )	\
	ERS_ASSERT_MSG( (min) < (val) && (val) < (max), \
        			val << " is not in (" << min << "," << max << ") range" )

#endif

