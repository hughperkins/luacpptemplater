project(luacpptemplater)
cmake_minimum_required(VERSION 3.4)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

option(BUILD_TESTS "Build unit-tests." OFF)
option(DEV_RUN_COG "Only EasyCL maintainers need this, otherwise set to 'OFF'." OFF)

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

include_directories(.)
set(TEMPLATESRC LuaTemplater.cpp util/luacpptemplater_stringhelper.cpp)
set(TEMPLATETESTS test/testLuaTemplater.cpp)

if(UNIX)
    set(EXTRA_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
    set(EXTRA_LIBS "dl")
endif()

if(APPLE)
    set_target_properties(luacpptemplater PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()

add_library(
    luacpptemplater STATIC
    LuaTemplater.cpp
    util/luacpptemplater_stringhelper)

if(BUILD_TESTS)
    add_executable(luacpptemplater_unittests ${TEMPLATETESTS} test/GtestGlobals.cpp thirdparty/gtest/gtest-all.cc)
    target_link_libraries(luacpptemplater_unittests luacpptemplater ${CONAN_LIBS} ${EXTRA_LIBS})
endif()

if(DEV_RUN_COG)
    add_custom_target(
        cog
        python ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cogapp/cog.py -q -I ${CMAKE_CURRENT_SOURCE_DIR}/cog-batteries -r ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h  ${CMAKE_CURRENT_SOURCE_DIR}/util/*.h  ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp
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

