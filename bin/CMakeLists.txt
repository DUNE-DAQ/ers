include_directories(${CMAKE_SOURCE_DIR}/ers)

add_executable(config config.cxx)
target_link_libraries(config ${CMAKE_DL_LIBS} ers)

install(
    TARGETS config
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
