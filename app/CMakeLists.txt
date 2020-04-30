set(ALL_APP_LIBS
  main_lib
)

add_executable(spammer_check_exe main.cpp)
set_target_properties(spammer_check_exe PROPERTIES OUTPUT_NAME spammer_check)

target_compile_features(spammer_check_exe PRIVATE cxx_std_17)
target_link_libraries(spammer_check_exe ${ALL_APP_LIBS})