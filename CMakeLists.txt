# DUNE DAQ modification notice:
# This file has been modified from the original ATLAS ers source for the DUNE DAQ project.
# Fork baseline commit: 8267df82a4f6fe6bf02c4014923eba19eddc4614 (2020-04-14).
# Renamed since fork: no.
#
# Original copyright:
# Copyright (C) 2001-2020 CERN for the benefit of the ATLAS collaboration.
# Licensed under the Apache License, Version 2.0.

cmake_minimum_required(VERSION 3.12)
project(ers VERSION 1.5.5)

find_package(daq-cmake REQUIRED )

daq_setup_environment()

find_package(Boost COMPONENTS unit_test_framework program_options regex REQUIRED)
find_package(Protobuf  REQUIRED)
find_package(absl  REQUIRED)

daq_protobuf_codegen( *.proto )

daq_add_library(*.cpp LINK_LIBRARIES Boost::regex pthread dl ${PROTOBUF_LIBRARY} absl::status)

daq_add_python_bindings(*.cpp GENERATE_STUBS)

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
daq_add_application( ers_inheritance ers_inheritance.cxx TEST LINK_LIBRARIES ers )
daq_add_application( ers_schema_test ers_schema_test.cxx TEST LINK_LIBRARIES ers )
daq_install()
