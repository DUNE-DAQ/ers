/*
 * DUNE DAQ modification notice:
 * This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
 * Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
 * Renamed since fork: yes (from ers/internal/Util.h to include/ers/internal/Util.hpp).
 *
 * Original copyright:
 * Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
 * Licensed under the Apache License, Version 2.0.
 */

/*
 *  Util.h
 *  ers
 *
 *  Created by Serguei Kolos on 26.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

/** \file Util.h This file defines some common utility functions for ERS.
  * \author Serguei Kolos
  * \brief ers header file 
  */

#ifndef ERS_UTIL_H
#define ERS_UTIL_H

#include <string>
#include <vector>

namespace ers
{    
    void tokenize(	const std::string & text,
    			const std::string & separators,
                        std::vector<std::string> & tokens );
    
    int read_from_environment( const char * name, int default_value );
    
    const char * read_from_environment( const char * name, const char * default_value );
}

#endif
