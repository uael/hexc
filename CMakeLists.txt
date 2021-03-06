cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(hexc)

option(COVERAGE "Turn on COVERAGE support" OFF)

if(COVERAGE AND NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
else()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -DDEBUG")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -fomit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-function")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-value -Wno-unused-result -Wno-missing-braces")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-char-subscripts")
endif()

if(MSVC)
    set(CMAKE_FLAGS
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
    )
    foreach(CMAKE_FLAG ${CMAKE_FLAGS})
        string(REPLACE "/MD" "/MT" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
        string(REPLACE "/MDd" "/MTd" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
    endforeach()
endif()

set(hex_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(hex_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)
set(hex_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(hex_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(hex_VENDOR_DIR ${CMAKE_CURRENT_LIST_DIR}/vendor)

file(GLOB_RECURSE hex_HEADERS ${hex_HEADERS} ${hex_INCLUDE_DIR}/${PROJECT_NAME}/*.h)
file(GLOB_RECURSE hex_SOURCES ${hex_SOURCES} ${hex_LIB_DIR}/*.c)

add_library(${PROJECT_NAME} ${hex_SOURCES} ${hex_HEADERS} ${hex_INCLUDE_DIR}/${PROJECT_NAME}.h)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${hex_HEADERS}")
target_include_directories(${PROJECT_NAME} PUBLIC ${hex_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${hex_LIB_DIR})

file(GLOB_RECURSE hex_SOURCES ${hex_SOURCES} ${hex_SOURCE_DIR}/*.c)

add_executable(hex ${hex_SOURCES})
target_link_libraries(hex ${PROJECT_NAME})

enable_testing()
add_subdirectory(${hex_TEST_DIR})
add_subdirectory(${hex_VENDOR_DIR})

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
install(FILES ${hex_INCLUDE_DIR}/${PROJECT_NAME}.h
    DESTINATION include
)