/*
 *  DefaultStream.cxx
 *  ERS
 *
 *  Created by Matthias Wiesmann on 21.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
#include <iostream>

#include "ers/DefaultStream.h"
#include "ers/StreamFactory.h"
#include "ers/InvalidReferenceIssue.h"


const char* const ers::DefaultStream::KEY = "default" ; 

namespace {
    ers::Stream *create_stream(const std::string &protocol, const std::string &uri) { 
        (void) uri ;
        if (protocol==ers::DefaultStream::KEY) {
           return new ers::DefaultStream() ;  
        } 
	return 0 ; 
    } // 
    bool registered = ers::StreamFactory::instance()->register_factory(ers::DefaultStream::KEY,create_stream) ;
} // anonymous namespace

ers::DefaultStream::DefaultStream() {} 
ers::DefaultStream::~DefaultStream() {} 


void ers::DefaultStream::send(const Issue *issue_ptr) {
    ERS_PRE_CHECK_PTR(issue_ptr); 
    try {
            const string_map_type *table = issue_ptr->get_value_table(); 
            const std::string message_str = issue_ptr->get_value(Issue::MESSAGE_KEY) ;
            const std::string severity_str = issue_ptr->get_value(Issue::SEVERITY_KEY) ;
            std::cerr << ">>> " << message_str << " (" << severity_str << ") <<<" << std::endl ; 
	    std::cerr << "-----------" << std::endl ; 
            for(string_map_type::const_iterator pos = table->begin();pos!=table->end();++pos) {
                const std::string &key = pos->first ; 
                const std::string &value = pos->second ; 
                std::cerr  << key << "=\t\"" << value << '\"' << std::endl ;
            } // for
            std::cerr << "-----------" << std::endl ; 
        } catch (std::ios_base::failure &ex) {
            throw ers::Issue(ERS_HERE,ers_error,&ex); 
        } // catch generic I/O errors 
   
} // send

