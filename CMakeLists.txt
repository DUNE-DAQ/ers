cmake_minimum_required(VERSION 3.12)
project(ers VERSION 1.5.1)

find_package(daq-cmake REQUIRED )

daq_setup_environment()

find_package(Boost COMPONENTS unit_test_framework program_options regex REQUIRED)
find_package(Protobuf  REQUIRED)
find_package(absl  REQUIRED)
find_package(cetlib REQUIRED)   # Uses the daq-buildtools/cmake/Findcetlib.cmake
find_package(cetlib_except REQUIRED)   # Uses the daq-buildtools/cmake/Findcetlib.cmake

daq_protobuf_codegen( *.proto )

daq_add_library(*.cpp LINK_LIBRARIES Boost::regex pthread dl ${PROTOBUF_LIBRARY} ${CETLIB} ${CETLIB_EXCEPT} absl::status)

daq_add_python_bindings(*.cpp LINK_LIBRARIES ${PROJECT_NAME})

daq_add_application( ers_config ers_config.cxx LINK_LIBRARIES ers )

daq_add_plugin( abort ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( exit ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( filter ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( glock ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( lock ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( null ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( rfilter ersOutStream LINK_LIBRARIES ers )
# daq_add_plugin( StandardStream ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( throttle ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( throw ersOutStream LINK_LIBRARIES ers )


daq_add_plugin( file ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( stdout ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( stderr ersOutStream LINK_LIBRARIES ers )

daq_add_plugin( lfile ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( lstdout ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( lstderr ersOutStream LINK_LIBRARIES ers )

daq_add_plugin( ffile ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( fstdout ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( fstderr ersOutStream LINK_LIBRARIES ers )

daq_add_plugin( lffile ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( lfstdout ersOutStream LINK_LIBRARIES ers )
daq_add_plugin( lfstderr ersOutStream LINK_LIBRARIES ers )

daq_add_application( ers_receiver ers_receiver.cxx TEST LINK_LIBRARIES ers )
daq_add_application( ers_test ers_test.cxx TEST LINK_LIBRARIES ers )
daq_add_application( ers_inheritance ers_inheritance.cxx TEST LINK_LIBRARIES ers )
daq_add_application( ers_schema_test ers_schema_test.cxx TEST LINK_LIBRARIES ers )
daq_install()
