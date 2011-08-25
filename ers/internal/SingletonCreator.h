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
	static bool	    s_instance_created;
        
        static T * create( )
        {
	    boost::mutex::scoped_lock lock(s_mutex);
	    T * instance;
            {
		if ( !s_instance_created )
		{
		    instance = new T();
		    s_instance_created = true;
		}
	    }
	    return instance;
        }
    };
    
    template <class T>
    boost::mutex 	SingletonCreator<T>::s_mutex;
    
    template <class T>
    bool 		SingletonCreator<T>::s_instance_created;
}

#endif
