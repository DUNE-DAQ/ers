include_directories(${CMAKE_SOURCE_DIR}/ers)

add_executable(ers_test test.cxx)
target_link_libraries(ers_test ${CMAKE_DL_LIBS} ers pthread)

add_executable(ers_receiver receiver.cxx)
target_link_libraries(ers_receiver ${CMAKE_DL_LIBS} ers pthread)
