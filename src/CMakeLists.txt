set(SPAMMER_CHECK_SOURCE
  format.cpp
  dns.cpp
)

set(SPAMMER_CHECK_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)

set(SPAMMER_CHECK_HEADERS
  ${SPAMMER_CHECK_INCLUDE_DIR}/format.hpp
  ${SPAMMER_CHECK_INCLUDE_DIR}/dns.hpp
)

add_library(
  main_lib 
  STATIC
  ${SPAMMER_CHECK_SOURCE}
  ${SPAMMER_CHECK_HEADERS}
)
target_include_directories(main_lib PUBLIC ${SPAMMER_CHECK_INCLUDE_DIR})
target_compile_features(main_lib PUBLIC cxx_std_17)
