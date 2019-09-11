
project(muduo C CXX)
cmake_minimum_required(VERSION 2.8.7)

set(USR_LOCAL "/usr/local")

# turn on C++11
if(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -w -g")
endif()

file(GLOB SRC_CODE_CPP
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp)


include_directories(${USR_LOCAL}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})




# link_directories(${USR_LOCAL}/lib)
# link_directories(${CMAKE_CURRENT_LIST_DIR}/../tools/lib)

list(APPEND TARGET_LIB  pthread  boost_thread boost_system  CppUTest CppUTestExt)

#list(APPEND COMPILE_CODE ${SRC_CODE_CPP} ${SRC_CODE_H})
list(APPEND COMPILE_CODE ${SRC_CODE_CPP} )


add_executable(muduo.bin ./test.cpp ${COMPILE_CODE})
target_link_libraries(muduo.bin ${TARGET_LIB})



#set_target_properties(cloud_sch.bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
#foreach(SAMPLE_NAME ${SAMPLE_CODE})
#    #unpack the dir "/"
#    string(REPLACE "/" ";" SEXY_LIST ${SAMPLE_NAME})
#    list(GET SEXY_LIST -1 TEST_CASE_NAME)
#    #get the file name without suffix
#    string(REPLACE "." ";" SEXY_LIST ${TEST_CASE_NAME})
#    list(GET SEXY_LIST 0 TEST_CASE_NAME)
#    CUDA_ADD_EXECUTABLE(${TEST_CASE_NAME}  ${SAMPLE_NAME} ${AVS_CODE} ${PIL_CODE} ${APP_CODE} ${API_CODE})
#    target_link_libraries(${TEST_CASE_NAME} -Wl,--whole-archive ${TARGET_LIB} -Wl,--no-whole-archive)
#    set_target_properties(${TEST_CASE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
#endforeach()