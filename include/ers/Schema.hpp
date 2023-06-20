#ifndef ERS_SCHEMA_HPP
#define ERS_SCHEMA_HPP 

#include <ers/issue.pb.h>
#include <ers/Issue.hpp>

namespace ers {

  // converters with allocation
  dunedaq::ersschema::Context to_schema( const Context & );

  dunedaq::ersschema::SimpleIssue to_schema( const Issue & );
  dunedaq::ersschema::IssueChain to_schema_chain(const Issue & );

  //overloads for uniformity 
  void to_schema ( const Context &, dunedaq::ersschema::Context & );
  void to_schema ( const Issue &,   dunedaq::ersschema::SimpleIssue & );
  void to_schema ( const Issue &,   dunedaq::ersschema::IssueChain & );

} // ers namespace

#endif  //ERS_SCHEMA_HPP
