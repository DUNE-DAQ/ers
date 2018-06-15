
tdaq_package()

tdaq_add_library(ers 
  src/*.cxx
  LINK_LIBRARIES PUBLIC Boost::thread Boost::date_time Boost::system dl)

tdaq_add_library(ErsBaseStreams MODULE
  src/streams/*.cxx
  LINK_LIBRARIES ers Boost::regex)

tdaq_add_library(erspy MODULE 
  python/erspy.cxx
  LINK_LIBRARIES PRIVATE Boost::python ers
  INCLUDE_DIRECTORIES PRIVATE PythonLibs)

tdaq_add_python_files(python/ers.py)

tdaq_add_executable(ers_test         test/test.cxx     NOINSTALL LINK_LIBRARIES ers)
tdaq_add_executable(ers_print_config bin/config.cxx    LINK_LIBRARIES ers)
tdaq_add_executable(ers_receiver     test/receiver.cxx LINK_LIBRARIES ers pthread)

add_test(NAME ers_check COMMAND  ers_test)
