add_executable(lib_test
  format.cpp
)
target_link_libraries(lib_test main_lib doctest)