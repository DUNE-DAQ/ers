#include "issue.pb.h"
#include <google/protobuf/util/json_util.h>


#include <boost/asio/ip/host_name.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>


using namespace std;
using namespace google::protobuf::util;


int main( int argc, char* argv[] ) {

  GOOGLE_PROTOBUF_VERIFY_VERSION;
  
  Context context;
  context.set_cwd("my_work_area");
  context.set_file_name("lost.cpp");
  context.set_function_name("test");
  context.set_host_name(boost::asio::ip::host_name());
  context.set_line_number(93);
  context.set_package_name("ers");

  context.set_process_id(100);
  context.set_thread_id(12345);
  context.set_user_id(12);
  context.set_user_name("maroda");
  context.set_application_name("ers_schema_test");

  IssueObject issue;
  (*issue.mutable_context())=context;
  issue.set_name("test_issue");
  issue.set_message("This is a message of test");
  issue.set_severity(Severity::LOG);

  (*issue.mutable_parameters())["p"]="perfect";
    
  google::protobuf::Timestamp time;
  time.set_seconds( chrono::system_clock::now().time_since_epoch().count() );

  (*issue.mutable_time())=time;
  


  
  string serial;
  
  context.SerializeToString( & serial );

  Context reco;
  reco.ParseFromString(serial.c_str());

 
  
  if ( context.cwd() == reco.cwd() ) {
    cout << "success" << endl;
  }
  else {
    cout << "failure" << endl;
  }

  JsonPrintOptions opt;
  opt.add_whitespace = true;
  opt.preserve_proto_field_names = true;
  
  string json;
  MessageToJsonString(issue, & json, opt);
  json += '}';
  cout << json << endl ;

  IssueObject reco_issue;

  JsonStringToMessage( json, & reco_issue);
  

  if ( issue.name() == reco_issue.name() ) {
    cout << "json success" << endl ;
  } else {
    cout << "Jons failure" << endl;
  }
    
  
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
