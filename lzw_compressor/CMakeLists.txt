cmake_minimum_required(VERSION 3.22)

# -----------------------------------------------------------------------------------------

set(encode_project lzw)
set(decode_project unlzw)

# -----------------------------------------------------------------------------------------

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# -----------------------------------------------------------------------------------------

add_subdirectory(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach(SRC ${SRC_LIST})
        target_compile_features(${SRC} PUBLIC cxx_std_20)
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${SRC})
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

        install(TARGETS ${SRC} DESTINATION bin)
endforeach()

# -----------------------------------------------------------------------------------------

add_executable(${encode_project} encode.cpp)
target_compile_features(${encode_project} PUBLIC cxx_std_20 )
target_link_libraries(${encode_project} ${SRC_LIST})
target_link_libraries(${encode_project} debug_utils Boost::program_options)
target_include_directories(${encode_project} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# -----------------------------------------------------------------------------------------

# add_executable(${decode_project} decode.cpp)
# target_compile_features(${decode_project} PUBLIC cxx_std_20 )
# target_link_libraries(${decode_project} ${SRC_LIST})
# target_link_libraries(${decode_project} debug_utils)
# target_include_directories(${decode_project} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
