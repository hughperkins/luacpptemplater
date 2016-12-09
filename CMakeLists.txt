project(luacpptemplater)
cmake_minimum_required(VERSION 3.4)

option(BUILD_TESTS "Build unit-tests." OFF)
option(DEV_RUN_COG "Only EasyCL maintainers need this, otherwise set to 'OFF'." OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_SOURCE_DIR}/dist"
        CACHE PATH
        "Installation prefix, default 'dist'"
        FORCE)
endif()

# https://cmake.org/Wiki/CMake_RPATH_handling
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_MACOSX_RPATH TRUE)

include_directories(include)
include_directories(SYSTEM thirdparty/gtest)

set(TEMPLATETESTS test/testLuaCppTemplater.cpp)

if(UNIX)
    set(EXTRA_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
    set(EXTRA_LIBS "dl")
endif()

if(APPLE)
    set_target_properties(luacpptemplater PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()

set(LCT_SRCS
    src/LuaCppTemplater/LuaCppTemplater.cpp
)

add_library(luacpptemplater STATIC ${LCT_SRCS})

if(BUILD_TESTS)
    add_executable(luacpptemplater_unittests
        ${LCT_SRCS}
        test/testLuaCppTemplater.cpp
        thirdparty/gtest/gtest-all.cc
        thirdparty/gtest/gtest_main.cc
    )
    include_directories(test)
    target_link_libraries(luacpptemplater_unittests
        luacpptemplater
        ${CONAN_LIBS}
        ${EXTRA_LIBS}
        Threads::Threads
    )
endif()

#
# make sure that you have installed cog via
#     pip install cogapp
#
if(DEV_RUN_COG)
    add_custom_target(
        cog
        cog.py -I ${CMAKE_CURRENT_SOURCE_DIR}/cog-batteries -r ${CMAKE_CURRENT_SOURCE_DIR}/src/LuaCppTemplater/*.cpp
    )
    add_dependencies(luacpptemplater cog)
endif()

install(
    TARGETS luacpptemplater
    EXPORT luacpptemplater-targets
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

file(GLOB HEADERS_ROOT *.h)
install(FILES ${HEADERS_ROOT} DESTINATION include/luacpptemplater)
install(EXPORT luacpptemplater-targets DESTINATION lib/luacpptemplater)

if(BUILD_TESTS)
   install(TARGETS luacpptemplater_unittests
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
      RUNTIME DESTINATION bin
  )
endif()

