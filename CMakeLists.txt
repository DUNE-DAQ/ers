cmake_minimum_required(VERSION 3.12)
project(ers VERSION 1.1.2)

find_package(daq-cmake REQUIRED )
find_package(Boost COMPONENTS regex REQUIRED) 

daq_setup_environment()

find_package(Boost 1.70.0 COMPONENTS unit_test_framework program_options REQUIRED)

daq_add_library(*.cpp LINK_LIBRARIES Boost::regex pthread dl)

daq_add_application( ers_config ers_config.cxx LINK_LIBRARIES ers )

daq_add_plugin( AbortStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( ExitStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( FilterStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( GlobalLockStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( LockStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( NullStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( RFilterStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( StandardStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( ThrottleStream ersStream LINK_LIBRARIES ers )
daq_add_plugin( ThrowStream ersStream LINK_LIBRARIES ers )

daq_add_application( ers_receiver ers_receiver.cxx TEST LINK_LIBRARIES ers )
daq_add_application( ers_test ers_test.cxx TEST LINK_LIBRARIES ers )

daq_install()