#set cmake version
cmake_minimum_required(VERSION 3.1)
project(muduo)
#set c++ version
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

#set source path
file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/main.cpp
                   ${PROJECT_SOURCE_DIR}/net/*.cpp
                   ${PROJECT_SOURCE_DIR}/base/*.cpp)
# aux_source_directory(${PROJECT_SOURCE_DIR}/net
                    #  ${PROJECT_SOURCE_DIR}/base SRC_LIST)
find_library(EXAMPLE_LIB soci_core PATHS ${PROJECT_SOURCE_DIR}/base/soci/lib)
#set head path
include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/net
                    ${PROJECT_SOURCE_DIR}/base
                    ${PROJECT_SOURCE_DIR}/base/soci/include)

#set output path
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})


#exe
add_executable(main ${SRC_LIST})
target_link_libraries(main pthread mysqlclient log4cpp ${EXAMPLE_LIB})