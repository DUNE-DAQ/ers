include_directories(${CMAKE_SOURCE_DIR}/ers)

add_executable(test test.cxx)
target_link_libraries(test ${CMAKE_DL_LIBS} ers pthread)

add_executable(receiver receiver.cxx)
target_link_libraries(receiver ${CMAKE_DL_LIBS} ers pthread)