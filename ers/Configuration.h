/*
 *  Configuration.h
 *  ers
 *
 *  Created by Serguei Kolos on 21.11.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file Configuration.h This file defines ERS Configuration class.
  * \author Serguei Kolos
  * \brief ers header and documentation file
  */

#ifndef ERS_CONFIGURATION_H
#define ERS_CONFIGURATION_H


#include <iostream>

/** \file StreamFactory.h This file defines the StreamFactory class, 
  * which is responsible for registration and creation of ERS streams.
  * \author Serguei Kolos
  * \brief ers header and documentation file 
  */

namespace ers
{
    
    /** The \c Configuration class is responsible for configuring the ERS outout.
      * 
      * \author Serguei Kolos
      * \version 1.2
      * \brief Manager of ERS streams. 
      * \see ers::debug
      * \see ers::error
      * \see ers::fatal
      * \see ers::infomation
      * \see ers::warning
      */
    
    class Configuration
    {	
	friend std::ostream & operator<<( std::ostream &, const ers::Configuration & );
        
      public:
	        
  	static Configuration & instance();	/**< \brief return the singleton */
        
        int debug_level() const			/**< \brief returns current debug level */
        { return m_debug_level; }
        
        int verbosity_level() const		/**< \brief returns current verbosity level */
        { return m_verbosity_level; }
        
        int & debug_level()			/**< \brief can be used to set the current debug level */
        { return m_debug_level; }
        
        int & verbosity_level()			/**< \brief can be used to set the current verbosity level */
        { return m_verbosity_level; }
        
      private:	
	Configuration( );

    	int m_debug_level;		/**< \brief current active level for the debug stream */	
    	int m_verbosity_level;		/**< \brief current verbosity level for all streams */
    };
    
    std::ostream & operator<<( std::ostream &, const ers::Configuration & );
}

#endif

