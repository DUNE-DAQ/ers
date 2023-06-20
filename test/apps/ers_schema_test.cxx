#include "ers/Schema.hpp"
#include <google/protobuf/util/json_util.h>


#include <boost/asio/ip/host_name.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>


using namespace std;
using namespace google::protobuf::util;

using namespace dunedaq::ersschema;

#include "ers/ers.hpp"

ERS_DECLARE_ISSUE( ersschema,
		   TestIssue,
		   "this is issue with ID: " << id,
		   ((int)id)
		   )
		   

int main( int argc, char* argv[] ) {

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ersschema::TestIssue first(ERS_HERE, 0);
  ersschema::TestIssue second(ERS_HERE, 1, first);
  ersschema::TestIssue third(ERS_HERE, 2, second);
  
  dunedaq::ersschema::IssueChain schema;
  to_schema(third, schema);
  schema.set_session("None");
  schema.set_application("ers_schema_test");

  JsonPrintOptions opt;
  opt.add_whitespace = true;
  opt.preserve_proto_field_names = true;
  
  string json;
  MessageToJsonString(schema, & json, opt);
  cout << json << endl ;

  // google::protobuf::Timestamp time;
  // time.set_seconds( chrono::system_clock::now().time_since_epoch().count() );
  // (*issue.mutable_time())=time;

    
  string serial;
  
  schema.SerializeToString( & serial );

  IssueChain reco;
  reco.ParseFromString(serial.c_str());

  if ( schema.final().severity() == reco.final().severity() ) {
    cout << "success" << endl;
  }
  else {
    cout << "failure" << endl;
  }

  // JsonStringToMessage( json, & reco_issue);
  

  // if ( issue.name() == reco_issue.name() ) {
  //   cout << "json success" << endl ;
  // } else {
  //   cout << "Jons failure" << endl;
  // }
    
  
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
