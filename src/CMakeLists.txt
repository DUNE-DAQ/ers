
find_package(Boost COMPONENTS regex REQUIRED) 

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB source_files "*.cxx")
add_library(ers SHARED ${source_files})
target_link_libraries(ers pthread dl)



message(${Boost_REGEX_LIBRARIES})

file(GLOB streams_srcs "streams/*.cxx")
add_library(ErsBaseStreams MODULE ${streams_srcs})
target_link_libraries(ErsBaseStreams ers Boost::regex)


