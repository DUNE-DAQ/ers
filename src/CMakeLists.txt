
file(GLOB source_files "*.cxx")
add_library(ers SHARED ${source_files})
target_link_libraries(ers pthread dl)
install(TARGETS ers
        LIBRARY
          DESTINATION lib
        PUBLIC_HEADER
          DESTINATION include)

find_package(Boost COMPONENTS regex) 
message(${Boost_REGEX_LIBRARIES})

file(GLOB streams_srcs "streams/*.cxx")
add_library(ErsBaseStreams MODULE ${streams_srcs})
target_link_libraries(ErsBaseStreams ers Boost::regex)
install(TARGETS ErsBaseStreams LIBRARY DESTINATION lib)

