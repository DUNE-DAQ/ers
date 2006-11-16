/*
 *  IssueFactory.h
 *  ers
 *
 *  Created by Matthias Wiesmann on 30.11.04.
 *  Modified by Serguei Kolos on 10.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#ifndef ERS_ISSUE_FACTORY
#define ERS_ISSUE_FACTORY

#include <string>
#include <vector>
#include <map>

/** \file IssueFactory.h This file defines the IssueFactory class, 
  * which is responsible for registration and creation of user defined issues.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */
  
namespace ers
{

    class Issue; 
    class Context;
    
    /** This class implements the factory pattern for Issues.
      * The main responsability of this class is to keep track of the existing types of Issues
      * Each issue should register one factory method for instances of this class.
      * This is needed for deserializing of Issues. 
      * 
      * \author Matthias Wiesmann
      * \version 1.0
      * \brief Factory for all Issues 
      */
    
    class IssueFactory
    {
	typedef Issue * (*IssueCreator)( const ers::Context & );
	typedef std::map<std::string,IssueCreator> FunctionMap;

      public:
	static IssueFactory & instance();					/**< \brief method to access singleton */

	Issue * create( const std::string & name,
        		const Context & context ) const ;			/**< \brief build an empty issue for a given name */
	
        Issue * create( const std::string & name,
        		const Context & context,
                        long  time,
                        const std::string & message,
                        const std::vector<std::string> & qualifiers,
                        const std::map<std::string, std::string> & parameters ) const ;		/**< \brief build issue out of all the given parameters */
	
        void register_issue( const std::string & name, IssueCreator creator );	/**< \brief register an issue factory */
      
      private:
	IssueFactory()
        { ; }
        
	FunctionMap m_creators;     
    }; // IssueFactory

    std::ostream& operator<<(std::ostream&, const IssueFactory& factory);         /**< \brief streaming operator */
} // ers

#endif

