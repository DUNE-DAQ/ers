
tdaq_package()

tdaq_add_library(ers 
  src/*.cxx
  INCLUDE_DIRECTORIES Boost
  LINK_LIBRARIES pthread dl)

tdaq_add_library(ErsBaseStreams MODULE
  src/streams/*.cxx
  LINK_LIBRARIES ers Boost::regex)

find_package(Python QUIET REQUIRED COMPONENTS Development)

tdaq_add_library(erspy MODULE 
  python/erspy.cxx
  LINK_LIBRARIES PRIVATE Boost::python ers Python::Module)

tdaq_add_python_files(python/ers.py)

tdaq_add_executable(ers_test         test/test.cxx     NOINSTALL LINK_LIBRARIES ers)
tdaq_add_executable(ers_print_config bin/config.cxx    LINK_LIBRARIES ers)
tdaq_add_executable(ers_receiver     test/receiver.cxx LINK_LIBRARIES ers)

add_test(NAME ers_check COMMAND ers_test 8)
