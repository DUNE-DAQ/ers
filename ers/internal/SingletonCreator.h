#ifndef ERS_SINGLETON_CREATOR_H
#define ERS_SINGLETON_CREATOR_H

/** \file SingletonCreator.h This file defines SingletonCreator ERS class.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#include <boost/thread/mutex.hpp>

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
    struct SingletonCreator
    {
	static boost::mutex s_mutex;
	static T *	    s_instance;
        
        static T * create( )
        {
	    boost::mutex::scoped_lock lock(s_mutex);
	    if ( !s_instance )
	    {
		s_instance = new T();
	    }
	    return s_instance;
        }
    };
    
    template <class T>
    boost::mutex 	SingletonCreator<T>::s_mutex;
    
    template <class T>
    T * 		SingletonCreator<T>::s_instance;
}

#endif
