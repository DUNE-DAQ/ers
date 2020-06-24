find_package(Boost COMPONENTS regex REQUIRED) 
message(STATUS ${Boost_REGEX_LIBRARIES})

file(GLOB source_files "*.cxx")
add_library(ers SHARED ${source_files})
target_link_libraries(ers pthread dl)
# The $<> stuff below are generator expressions. At build time, we
# will look for includes in the source directory, but for dependencies
# that are compiling/linking against ers, they will need to look in
# the install directory (the argument of $<INSTALL_INTERFACE> is
# interpreted relative to ${CMAKE_INSTALL_PREFIX}. See:
#
# https://cliutils.gitlab.io/modern-cmake/chapters/basics/functions.html#generator-expressions
target_include_directories(ers
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

file(GLOB streams_srcs "streams/*.cxx")
add_library(ErsBaseStreams MODULE ${streams_srcs})
target_link_libraries(ErsBaseStreams ers Boost::regex)

include(GNUInstallDirs)

install(
    TARGETS ers ErsBaseStreams
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
