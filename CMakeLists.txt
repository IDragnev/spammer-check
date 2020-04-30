cmake_minimum_required(VERSION 3.10)

project(spammer-check
LANGUAGES CXX
)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

endif()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(core_source  REGULAR_EXPRESSION src/.*)
source_group(core_headers REGULAR_EXPRESSION include/.*)

add_subdirectory(app)