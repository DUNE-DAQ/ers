/*
 *  EntityNotFoundIssue.h
 *  Test
 *
 *  Created by Matthias Wiesmann on 09.02.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include "ers/Issue.h"

namespace ers {
    
    class EntityNotFoundIssue : public Issue {
	
protected:
	EntityNotFoundIssue(const ers::Context &c, ers::ers_severity s) ; 
public:
	static const char* const CLASS_NAME ; 
	static const char* const ENTITY_NAME_KEY ; 
	static const char* const ENTITY_TYPE_KEY ; 
	static const char* const ENTITY_CONTEXT_KEY ;
	EntityNotFoundIssue();
	EntityNotFoundIssue(const ers::Context &c, ers::ers_severity s, 
			    const std::string &entity_name, 
			    const std::string &entity_type,
			    const std::string &entity_context) ; 
	virtual const char*get_class_name() const throw() ;
	
    } ; // EntityNotFoundIssue
    
} // ers