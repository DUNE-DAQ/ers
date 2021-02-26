/*
 *  Configuration.cxx
 *  ERS
 *
 *  Created by Serguei Kolos on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
#include <cstdio>
#include <iostream>

#include <ers/Configuration.hpp>
#include <ers/ers.hpp>
#include <ers/internal/SingletonCreator.hpp>
#include <ers/internal/Util.hpp>

/** This method returns the singleton instance. 
  * It should be used for every operation on the factory. 
  * \return a reference to the singleton instance 
  */
ers::Configuration &
ers::Configuration::instance()
{
    /**Singleton instance
      */
    static ers::Configuration * instance = ers::SingletonCreator<ers::Configuration>::create();
    
    return *instance;
}

/** Private constructor - can not be called by user code, use the \c instance() method instead
  * \see instance() 
  */
ers::Configuration::Configuration()
  : m_debug_level( 0 ),
    m_verbosity_level( 0 )
{
    m_debug_level = read_from_environment( "DUNEDAQ_ERS_DEBUG_LEVEL", m_debug_level );
    m_verbosity_level = read_from_environment( "DUNEDAQ_ERS_VERBOSITY_LEVEL", m_verbosity_level );
}

void 
ers::Configuration::verbosity_level( int verbosity_level )
{
    m_verbosity_level = verbosity_level;
}

std::ostream & 
ers::operator<<( std::ostream & out, const ers::Configuration & conf )
{
    out << "debug level = " << conf.m_debug_level << " verbosity level = " << conf.m_verbosity_level;
    return out;
}
