
find_package(Boost COMPONENTS regex preprocessor) 

file(GLOB source_files "*.cxx")
add_library(ers SHARED ${source_files})
target_link_libraries(ers pthread dl Boost::preprocessor)



message(${Boost_REGEX_LIBRARIES})

file(GLOB streams_srcs "streams/*.cxx")
add_library(ErsBaseStreams MODULE ${streams_srcs})
target_link_libraries(ErsBaseStreams ers Boost::regex)


