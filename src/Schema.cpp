#include <ers/Schema.hpp>

using namespace ers;

dunedaq::ersschema::Context to_schema( const Context & c ) {

  dunedaq::ersschema::Context out;
  to_schema( c, out);
  return out;
}

dunedaq::ersschema::SimpleIssue to_schema( const Issue & i ) {

  dunedaq::ersschema::SimpleIssue out;
  to_schema( i, out);
  return out;
}


dunedaq::ersschema::IssueChain to_schema_chain(const Issue & i) {

  dunedaq::ersschema::IssueChain out ;
  to_schema( i, out );
  return out;
}


void to_schema ( const Context & c, dunedaq::ersschema::Context & out) {

  out.set_cwd( c.cwd() );
  out.set_file_name( c.file_name() );
  out.set_function_name( c.function_name() );
  out.set_host_name( c.host_name() );
  out.set_line_number( c.line_number() );
  out.set_package_name( c.package_name() );
  
  out.set_process_id( c.process_id() );
  out.set_thread_id( c.thread_id() );
  out.set_user_id( c.user_id() );
  out.set_user_name( c.user_name() );
  out.set_application_name( c.application_name() );

}

void to_schema ( const Issue & i,   dunedaq::ersschema::SimpleIssue & out) {

  auto c = to_schema( i.context()) ;
  
  dunedaq::ersschema::SimpleIssue out;

  (*out.mutable_context())=c;

  out.set_name( i.get_class_name() );

  auto inheritance = i.get_class_inheritance();
  for ( auto & c : inheritance ) {
    out.add_inheritance(c);
  }
    
  out.set_message( i.message() ) ;
  out.set_severity( std::to_string( i.severity() ) );
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(i.ptime().time_since_epoch()).count();
  out.set_time(time);
    
  auto & params = (* out.mutable_parameters());
  for ( auto p : i.parameters() ) {
    params[p.first] = p.second;
  }
    
  return out;      
}


void to_schema ( const Issue & i,   dunedaq::ersschema::IssueChain & out) {

  (* out.mutable_final()) = to_schema(i);

  auto cause_ptr = i.cause();

  while ( cause_ptr ) {
    auto ptr = out.add_causes() ;
    to_schema( *cause_ptr, *ptr);
    cause_ptr = cause_ptr -> cause();
  }

}












