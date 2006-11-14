#ifndef ERS_SINGLETON_CREATOR_H
#define ERS_SINGLETON_CREATOR_H

/** \file SingletonCreator.h This file defines SingletonCreator ERS class.
  * \author Serguei Kolos
  * \brief ers header file 
  */

/*
 *  SingletonCreator.h
 *  ers
 *
 *  Created by Serguei Kolos on 10.05.06.
 *  Copyright 2005 CERN. All rights reserved.
 *  This class creates singleton instance during static initialization
 *  and muilti-threading issues are left aside at least for this phase
 *  (idea is taken from the boost thread package)
 */
 
namespace ers
{
    template <class T>
    struct SingletonCreator
    {
    	SingletonCreator() { T::instance(); }
        inline void dummy() const { ; }
    };
}

#endif
