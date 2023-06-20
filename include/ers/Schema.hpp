/*
 *  Schema.hpp
 *  This file contains untilities to conver an Issue in it corresponding schema object
 *
 */ 

#ifndef ERS_SCHEMA_HPP
#define ERS_SCHEMA_HPP 

#include <ers/issue.pb.h>
#include <ers/Issue.hpp>

namespace ers {
  dunedaq::ersschema::Context to_schema( const Context & c );
  dunedaq::ersschema::SimpleIssue to_schema( const Issue & i ); 
  dunedaq::ersschema::IssueChain to_schema_chain(const Issue & i);

  void to_schema ( const Context & c, dunedaq::ersschema::Context & out);
  void to_schema ( const Issue & i,   dunedaq::ersschema::SimpleIssue & out);
  void to_schema ( const Issue & i,   dunedaq::ersschema::IssueChain & out);
  
} // ers namespace

#endif  //ERS_SCHEMA_HPP
