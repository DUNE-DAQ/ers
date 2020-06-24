include_directories(${CMAKE_SOURCE_DIR}/ers)

add_executable(ers-test test.cxx)
target_link_libraries(ers-test ${CMAKE_DL_LIBS} ers pthread)

add_executable(receiver receiver.cxx)
target_link_libraries(receiver ${CMAKE_DL_LIBS} ers pthread)
