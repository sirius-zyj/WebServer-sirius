#set cmake version
cmake_minimum_required(VERSION 3.1)
project(muduo)
#set c++ version
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

#set source path
file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/main.cpp
                   ${PROJECT_SOURCE_DIR}/net/*.cpp
                   ${PROJECT_SOURCE_DIR}/net/member/*.cpp
                   ${PROJECT_SOURCE_DIR}/net/member/sub_member/*.cpp
                   ${PROJECT_SOURCE_DIR}/net/member/sub_member/http/*.cpp
                   ${PROJECT_SOURCE_DIR}/plugin/*.cpp
                   ${PROJECT_SOURCE_DIR}/plugin/log/logger.cpp
                   ${PROJECT_SOURCE_DIR}/plugin/buffer/buffer.cpp)


#set head path
include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/net
                    ${PROJECT_SOURCE_DIR}/net/member
                    ${PROJECT_SOURCE_DIR}/net/member/sub_member
                    ${PROJECT_SOURCE_DIR}/net/member/sub_member/http
                    ${PROJECT_SOURCE_DIR}/plugin
                    ${PROJECT_SOURCE_DIR}/plugin/soci/include
                    ${PROJECT_SOURCE_DIR}/plugin/log
                    ${PROJECT_SOURCE_DIR}/plugin/buffer)

find_library(SOCI_LIB soci_core PATHS ${PROJECT_SOURCE_DIR}/plugin/soci/lib)
find_library(LOG4CPP_LIB log4cpp PATHS ${PROJECT_SOURCE_DIR}/plugin/log/lib)
                    

#set output path
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

#exe
add_executable(main ${SRC_LIST})
target_link_libraries(main pthread mysqlclient ${LOG4CPP_LIB} ${SOCI_LIB})