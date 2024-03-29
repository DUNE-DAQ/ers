/*
 *  IssueFactory.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 30.11.04.
 *  Modified by Serguei Kolos on 10.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/ers.hpp>
#include <ers/IssueFactory.hpp>
#include <ers/StreamFactory.hpp>
#include <ers/AnyIssue.hpp>
#include <ers/internal/SingletonCreator.hpp>
#include <ers/internal/macro.hpp>

/** Returns the singleton instance of the factory.
  * \return a reference to the singleton instance 
  */
ers::IssueFactory &
ers::IssueFactory::instance()
{
    static ers::IssueFactory * instance = ers::SingletonCreator<ers::IssueFactory>::create();
    
    return *instance;
} // instance

/** Register an issue type with the factory 
  * \param name the name that will be used to lookup new instances 
  * \param creator a pointer to the function used to create new instance for that particular type of function
  */
void 
ers::IssueFactory::register_issue( const std::string & name, IssueCreator creator )
{
    FunctionMap::const_iterator it = m_creators.find(name); 
    if ( it == m_creators.end() )
    {
	m_creators[name] = creator;
    }
}

/** Builds an issue out of the name it was registered with 
  * \param name the name used to indentify the class 
  * \return an newly allocated instance of type \c name or AnyIssue 
  * \note If the requested type cannot be resolved an instance of type AnyIssue 
  */
ers::Issue * 
ers::IssueFactory::create(	const std::string & name,
				const ers::Context & context ) const
{
    FunctionMap::const_iterator it = m_creators.find(name); 
    if ( it == m_creators.end() )
    {
	ERS_INTERNAL_DEBUG( 1, "Creator for the \"" << name << "\" issue is not found" );
        return new ers::AnyIssue( name, context );
    }
    
    ERS_INTERNAL_DEBUG( 2, "Creating the \"" << name << "\" issue" );
    return (it->second)( context ); 
}


ers::Issue *
ers::IssueFactory::create(	const std::string & name,
				const std::list<std::string> & inheritance,
				const ers::Context & context,
                                Severity severity,
                                const system_clock::time_point & time,
				const std::string & message,
				const std::vector<std::string> & qualifiers,
				const ers::string_map & parameters,
                                const Issue * cause ) const
{
    ers::Issue * issue = create( name, context );
    issue->m_message = message;
    issue->m_severity = severity;
    issue->m_qualifiers = qualifiers;
    issue->m_values = parameters;
    issue->m_time = time;
    issue->m_cause.reset( cause );

    auto * any = dynamic_cast<ers::AnyIssue *>( issue );
    if ( any ) {
      any -> m_inheritance = inheritance;
    }
    return issue;
}

