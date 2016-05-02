cmake_minimum_required(VERSION 2.8)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if (NOT CMAKE_BUILD_TYPE)
#    message("Setting build type to 'Release'")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

OPTION(BUILD_TESTS "Build unit-tests." OFF)
OPTION(DEV_RUN_COG "Only EasyCL maintainers need this, otherwise set to 'OFF'." OFF)
OPTION(PROVIDE_LUA_ENGINE "eg for Torch, we use Torch's engine." ON)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
    "${CMAKE_CURRENT_SOURCE_DIR}/dist" CACHE PATH "Installation prefix, default 'dist'" FORCE
   )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# https://cmake.org/Wiki/CMake_RPATH_handling
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_MACOSX_RPATH TRUE)

if(BUILD_TESTS AND NOT PROVIDE_LUA_ENGINE)
  # find_library(Lua51 REQUIRED)
  message("Error: if building tests, need to enable PROVIDE_LUA_ENGINE")
endif()

include_directories(.)
include_directories(thirdparty/lua-5.1.5/src)
set(TEMPLATESRC LuaTemplater.cpp util/cppluatemplater_stringhelper.cpp)
set(TEMPLATETESTS test/testLuaTemplater.cpp)

if(PROVIDE_LUA_ENGINE)
    file(STRINGS thirdparty/lua-5.1.5/files.txt lua_src1)
    foreach(source ${lua_src1})
        set(lua_src ${lua_src} thirdparty/lua-5.1.5/src/${source})
    endforeach()
endif(PROVIDE_LUA_ENGINE)

add_library(cppluatemplater SHARED LuaTemplater.cpp util/cppluatemplater_stringhelper ${lua_src})

if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

if(NOT PROVIDE_LUA_ENGINE)
  IF(APPLE)
    SET_TARGET_PROPERTIES(cppluatemplater PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  ENDIF(APPLE)
else()
  IF(APPLE)
    SET_TARGET_PROPERTIES(cppluatemplater PROPERTIES LINK_FLAGS "-single_module")
    ADD_DEFINITIONS(-DLUA_USE_LINUX)
  ENDIF(APPLE)
endif(NOT PROVIDE_LUA_ENGINE)

if(BUILD_TESTS)
  if(UNIX)
      add_library(cppluatemplater_gtest SHARED thirdparty/gtest/gtest-all.cc)
      target_link_libraries(cppluatemplater pthread)
  else()
      add_library(cppluatemplater_gtest thirdparty/gtest/gtest-all.cc)
  endif()

  add_executable(cppluatemplater_unittests ${TEMPLATETESTS} test/gtest_main.cpp test/GtestGlobals.cpp)
  target_link_libraries(cppluatemplater_unittests cppluatemplater_gtest cppluatemplater)
  target_include_directories(cppluatemplater_unittests PRIVATE thirdparty/gtest)
endif(BUILD_TESTS)

if(DEV_RUN_COG)
    add_custom_target(
        cog
        python ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cogapp/cog.py -q -I ${CMAKE_CURRENT_SOURCE_DIR}/cog-batteries -r ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h  ${CMAKE_CURRENT_SOURCE_DIR}/util/*.h  ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp
   )
    add_dependencies(cppluatemplater cog)
endif(DEV_RUN_COG)

INSTALL(TARGETS cppluatemplater EXPORT cppluatemplater-targets
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

FILE(GLOB HEADERS_ROOT *.h)
INSTALL(FILES ${HEADERS_ROOT} DESTINATION include/cppluatemplater)
INSTALL(EXPORT cppluatemplater-targets DESTINATION lib/cppluatemplater)
if(BUILD_TESTS)
   INSTALL(TARGETS cppluatemplater_gtest cppluatemplater_unittests
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
      RUNTIME DESTINATION bin
  )
endif(BUILD_TESTS)

