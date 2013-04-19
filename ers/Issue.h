/*
 *  Issue.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 08.12.04.
 *  Modified by Serguei Kolos on 02.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_ISSUE_H
#define ERS_ISSUE_H

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <chrono>

#include <ers/IssueFactory.h>
#include <ers/LocalContext.h>
#include <ers/Severity.h>

/** \file Issue.h This file defines the ers::Issue class, 
  * which is the base class for any user defined issue.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

using std::chrono::system_clock;

namespace ers
{
    class OutputStream;
        
    typedef std::map<std::string, std::string>	string_map;
    
    template<class T>
    class IssueRegistrator
    {
      public:
	IssueRegistrator()
	{ ers::IssueFactory::instance().register_issue( T::get_uid(), create ); }
        
      private:
	static ers::Issue * create( const Context & context )
	{ return new T( context ); }
    };
    
    /** This class is the root Issue object.
      *  The class does not contain any fields with information, instead everything is stored in a hashmap
      *  as key - value paris (both strings).
      *  The object contains utility methods to allow the manipulation of those key / values and
      *  code to insert common values into it, like time, compilation information, host information etc.
      *  For an example of how to build an actual subclass of issue look at the source of ExampleIssue.
      *  \author Matthias Wiesmann
      *  \version 1.1
      *  When the object is destroyed, it destroys the pointed issue. This means we can only chain issues that
      *  correctly implement the factory methods required by the IssueFactory class
      *  \see ers::IssueFactory
      *  \see ExampleIssue
      *  \brief Root Issue class
      */
    
    class Issue : public std::exception
    {
	friend class IssueFactory;

      public:        
	Issue(	const Context & context,
		const std::string & message = std::string() ); 
	
	Issue(	const Context & context,
		const std::exception & cause );

	Issue(	const Context & context,
		const std::string & message,
                const std::exception & cause ); 	
	
	Issue( const Issue & other );
	      
	virtual ~Issue() throw();
	
	virtual Issue * clone() const = 0;
	
        virtual const char * get_class_name() const = 0;	/**< \brief Get key for class (used for serialisation)*/
       	
        virtual void raise() const throw( std::exception ) = 0;
	
	void add_qualifier( const std::string & qualif );	/**< \brief adds a qualifier to the issue */
	
	const Issue * cause() const				/**< \brief return the cause Issue of this Issue */
	{ return m_cause.get(); }
        
	const Context & context() const				/**< \brief Context of the issue. */
        { return *(m_context.get()); }
        
        const std::string & message() const			/**< \brief General cause of the issue. */
	{ return m_message; }
        
	const std::vector<std::string> & qualifiers() const	/**< \brief return array of qualifiers */
        { return m_qualifiers; }
        
	const string_map & parameters() const /**< \brief return array of parameters */
        { return m_values; }
        
        ers::Severity severity() const				/**< \brief severity of the issue */
	{ return m_severity; }
        
	template <class Precision>
        std::string time(const std::string & format = "%y-%b-%d %H:%M:%S", bool isUTC = false) const;

	/**< \brief string representation of local time of the issue */
        template <class Precision>
        std::string localtime(const std::string & format = "%y-%b-%d %H:%M:%S") const
        { return time<Precision>(format, false); }
        
	/**< \brief string representation of UTC time of the issue */
	template <class Precision>
        std::string gmtime(const std::string & format = "%y-%b-%d %H:%M:%S") const
        { return time<Precision>(format, true); }
        
	std::time_t time_t() const;				/**< \brief seconds since 1 Jan 1970 */
        
	const system_clock::time_point & ptime() const		/**< \brief original time point of the issue */
	{ return m_time; }
        
        const char * what() const throw()			/**< \brief General cause of the issue. */
	{ return m_message.c_str(); }
        
	ers::Severity set_severity( ers::Severity severity ) const;

	void wrap_message( const std::string & begin, const std::string & end );
        
      protected:
        Issue(	Severity severity,
		const system_clock::time_point & time,
		const ers::Context & context,
		const std::string & message,
		const std::vector<std::string> & qualifiers,
		const std::map<std::string, std::string> & parameters,
		const ers::Issue * cause = 0 );
        
	template <typename T>
	void get_value( const std::string & key, T & value ) const;	/**< \brief Gets a value of any type, which defines the standard stream input operator */
	void get_value( const std::string & key, const char * & value ) const;
	void get_value( const std::string & key, std::string & value ) const;
	
	template <typename T>
	void set_value( const std::string & key, T value );	/**< \brief Sets a value of any type, which defines the standard stream output operator*/

	void set_message( const std::string & message )
	{ m_message = message; }
        
	void prepend_message( const std::string & message );
        
      private:        
        //////////////////////////////////////
        // Copy operation is not allowed
        //////////////////////////////////////
        Issue & operator=( const Issue & other );
					  
	std::unique_ptr<const Issue>	m_cause;		/**< \brief Issue that caused the current issue */
	std::unique_ptr<Context>	m_context;		/**< \brief Context of the current issue */
	std::string			m_message;		/**< \brief Issue's explanation text */
	std::vector<std::string>	m_qualifiers;		/**< \brief List of associated qualifiers */
	mutable Severity		m_severity;		/**< \brief Issue's severity */
	system_clock::time_point	m_time;			/**< \brief Time when issue was thrown */
	string_map			m_values;		/**< \brief List of user defined attributes. */	
    };

    std::ostream & operator<<( std::ostream &, const ers::Issue & );    
} // ers


#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/logical.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/comparison.hpp>

#define ERS_EMPTY

#define ERS_IS_EMPTY(x) 				ERS_IS_EMPTY_I(ERS_IS_EMPTY_HELPER x)
#define ERS_IS_EMPTY_I(contents) 			ERS_IS_EMPTY_II( contents() )
#define ERS_IS_EMPTY_II(contents) 			ERS_IS_EMPTY_III( contents )
#define ERS_IS_EMPTY_III(contents) 			BOOST_PP_TUPLE_ELEM( 2, 0, ( ERS_IS_EMPTY_DEF_ ## contents ) )
#define ERS_IS_EMPTY_HELPER() 				ERS_IS_EMPTY_TRUE
#define ERS_IS_EMPTY_DEF_ERS_IS_EMPTY_TRUE		1 ,
#define ERS_IS_EMPTY_DEF_ERS_IS_EMPTY_HELPER		0 ,

#define ERS_TYPE( tuple )				BOOST_PP_SEQ_HEAD(tuple)
#define ERS_NAME( tuple )				BOOST_PP_SEQ_TAIL(tuple)

#define ERS_ATTRIBUTE_NAME( _, __, tuple )		, ERS_NAME(tuple)

#define ERS_ATTRIBUTE_NAME_TYPE( _, __, tuple )		, ERS_TYPE(tuple) \
							  ERS_NAME(tuple)

#define ERS_ATTRIBUTE_SERIALIZATION( _, __, tuple )	set_value( BOOST_PP_STRINGIZE(ERS_NAME(tuple)), \
							ERS_NAME(tuple) );

#define ERS_ATTRIBUTE_ACCESSORS( _, __, tuple )		ERS_TYPE(tuple) \
							BOOST_PP_CAT( get_, ERS_NAME(tuple) ) () { \
								ERS_TYPE(tuple) val; \
								ers::Issue::get_value( BOOST_PP_STRINGIZE(ERS_NAME(tuple)), val ); \
								return val; \
							}
                                                                
#define ERS_SET_MESSAGE( message )			std::ostringstream out;\
							out << message;\
							prepend_message( out.str() );

#define	ERS_PRINT_LIST( decl, attributes )		BOOST_PP_SEQ_FOR_EACH( decl, _, attributes )

#define ERS_DECLARE_ISSUE_BASE( namespace_name, class_name, base_class_name, message, base_attributes, attributes ) \
namespace namespace_name { \
    class class_name : public base_class_name { \
      template <class> friend class ers::IssueRegistrator;\
      protected: \
	BOOST_PP_EXPR_IF( BOOST_PP_NOT_EQUAL( BOOST_PP_SEQ_SIZE( base_attributes attributes ), 0 ), class_name( const ers::Context & context ) : base_class_name( context ) { ; } )\
        \
	virtual void raise() const throw( std::exception ) { throw *this; } \
	static const char * get_uid() { return BOOST_PP_STRINGIZE( namespace_name::class_name ); } \
	virtual const char * get_class_name() const { return get_uid(); } \
	virtual base_class_name * clone() const { return new namespace_name::class_name( *this ); } \
      public: \
	virtual ~class_name() throw() { ; } \
	class_name( const ers::Context & context \
        	    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY base_attributes ) \
                    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY attributes ) ) \
          : base_class_name( context ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME, ERS_EMPTY base_attributes ) ) \
	{ \
          ERS_PRINT_LIST( ERS_ATTRIBUTE_SERIALIZATION, ERS_EMPTY attributes ) \
	  BOOST_PP_EXPR_IF( BOOST_PP_NOT( ERS_IS_EMPTY( ERS_EMPTY message ) ), ERS_SET_MESSAGE( ERS_EMPTY message ) )\
	} \
        \
	class_name( const ers::Context & context, \
        	    const std::string & msg \
        	    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY base_attributes ) \
                    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY attributes ) ) \
          : base_class_name( context, msg ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME, ERS_EMPTY base_attributes ) ) \
	{ \
          ERS_PRINT_LIST( ERS_ATTRIBUTE_SERIALIZATION, ERS_EMPTY attributes ) \
	} \
        \
	class_name( const ers::Context & context, \
        	    const std::string & msg \
        	    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY base_attributes ) \
                    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY attributes ), \
                    const std::exception & cause ) \
          : base_class_name( context, msg ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME, ERS_EMPTY base_attributes ), cause ) \
	{ \
          ERS_PRINT_LIST( ERS_ATTRIBUTE_SERIALIZATION, ERS_EMPTY attributes ) \
	} \
	class_name( const ers::Context & context \
        	    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY base_attributes ) \
                    ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME_TYPE, ERS_EMPTY attributes ), \
                    const std::exception & cause ) \
          : base_class_name( context ERS_PRINT_LIST( ERS_ATTRIBUTE_NAME, ERS_EMPTY base_attributes ), cause ) \
	{ \
          ERS_PRINT_LIST( ERS_ATTRIBUTE_SERIALIZATION, ERS_EMPTY attributes ) \
	  BOOST_PP_EXPR_IF( BOOST_PP_NOT( ERS_IS_EMPTY( ERS_EMPTY message ) ), ERS_SET_MESSAGE( ERS_EMPTY message ) )\
	} \
        \
	ERS_PRINT_LIST( ERS_ATTRIBUTE_ACCESSORS, ERS_EMPTY attributes ) \
    }; \
} \
namespace { \
	ers::IssueRegistrator<namespace_name::class_name> namespace_name##_##class_name##_instance; \
}

#define ERS_DECLARE_ISSUE( namespace_name, class_name, message, attributes ) \
	ERS_DECLARE_ISSUE_BASE( namespace_name, class_name, ers::Issue, ERS_EMPTY message, ERS_EMPTY, ERS_EMPTY attributes )
        
ERS_DECLARE_ISSUE(  ers,
		    NoValue,
		    "value for the \"" << key << "\" key is not set ",
		    ((std::string)key ) )

template <typename T>
void 
ers::Issue::get_value( const std::string & key, T & value ) const
{
    string_map::const_iterator it = m_values.find(key);
    if ( it == m_values.end() )
    {
	throw ers::NoValue( ERS_HERE, key );
    }
    std::istringstream in( it->second );
    in >> value;
}

template <typename T>
void 
ers::Issue::set_value( const std::string & key, T value )
{
    std::ostringstream out;
    out << value;
    m_values[key] = out.str();
}

template <class Precision>
std::string 
ers::Issue::time(const std::string & format, bool isUTC) const
{
    std::time_t t = time_t();
    std::tm tm = isUTC ? *gmtime_r(&t, &tm) : *localtime_r(&t, &tm);

    char buff[128];
    std::strftime(buff, 128 - 16, format.c_str(), &tm);

    auto c = std::chrono::duration_cast<Precision>(
			m_time.time_since_epoch()).count();
    double frac = c - (double)t*Precision::period::den;
    if (frac)
	sprintf(buff+strlen(buff), ",%.0f", frac);

    return buff;
}

#endif

