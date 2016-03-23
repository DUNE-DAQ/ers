
tdaq_package()

tdaq_add_library(ers 
  src/*.cxx
  LINK_LIBRARIES PUBLIC Boost::thread Boost::date_time Boost::system dl)

tdaq_add_library(ErsBaseStreams MODULE
  src/streams/*.cxx
  LINK_LIBRARIES ers)

tdaq_add_library(erspy MODULE 
  python/erspy.cxx
  LINK_LIBRARIES PRIVATE Boost::python ers
  INCLUDE_DIRECTORIES PRIVATE PythonLibs)

install(FILES python/ers.py COMPONENT ${TDAQ_PACKAGE_NAME}_noarch DESTINATION share/lib/python)

tdaq_add_executable(ers_test         test/test.cxx     LINK_LIBRARIES ers)
tdaq_add_executable(ers_print_config bin/config.cxx    LINK_LIBRARIES ers)
tdaq_add_executable(ers_receiver     test/receiver.cxx LINK_LIBRARIES ers)

add_test(NAME ers_check_target COMMAND  " ${CMAKE_CURRENT_SOURCE_DIR}/check_target.sh ${CMAKE_CURRENT_BINARY_DIR}")
