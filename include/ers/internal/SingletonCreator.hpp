/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from ers/internal/SingletonCreator.h to include/ers/internal/SingletonCreator.hpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

#ifndef ERS_SINGLETON_CREATOR_H
#define ERS_SINGLETON_CREATOR_H

/** \file SingletonCreator.h This file defines SingletonCreator ERS class.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#include <mutex>

/*
 *  SingletonCreator.h
 *  ers
 *
 *  Created by Serguei Kolos on 25.08.11.
 *  Copyright 2011 CERN. All rights reserved.
 *  This class creates singleton instance at the first use attempt
 *  in a threading-safe way.
 */
namespace ers
{
    template <class T>
    class SingletonCreator
    {
	static std::mutex s_mutex;
	static T *	  s_instance;
        
      public:
        static T * create( )
        {
	// on MacOS with gcc 4.2 the s_mutex is not yet created
        // at this moment which causes crash. This might be fixed 
        // when movining to newer gcc version
        #if !defined( __APPLE__) && !defined(__rtems__)
	    std::scoped_lock lock(s_mutex);
        #endif
	    if ( !s_instance )
	    {
		s_instance = new T();
	    }
	    return s_instance;
        }
    };
    
    template <class T>
    std::mutex 	SingletonCreator<T>::s_mutex;
    
    template <class T>
    T * 	SingletonCreator<T>::s_instance;
}

#endif
