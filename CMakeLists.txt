cmake_minimum_required(VERSION 3.30)
project(cyberpunk-cordoba-2077 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

# =============================================================================
# Opciones
# =============================================================================
option(CYBERPUNK_BUILD_TESTS "Build unit tests" ON)
option(CYBERPUNK_BUILD_BENCHMARKS "Build benchmarks" ON)
option(CYBERPUNK_ENABLE_COVERAGE "Enable coverage flags (local only)" OFF)

# =============================================================================
# Flags de compilación
# =============================================================================
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

# =============================================================================
# Biblioteca estática
# =============================================================================
add_library(cyberpunk_lib STATIC
  src/common/types.cpp
  src/model/gameModel.cpp
  src/controller/commandRegistry.cpp
  src/controller/helpCommand.cpp
  src/controller/statusCommand.cpp
  src/controller/unknownCommand.cpp
)

target_include_directories(cyberpunk_lib
  PUBLIC include
  PRIVATE src/controller
)

# =============================================================================
# Ejecutable principal
# =============================================================================
add_executable(cyberpunk2077
  src/main.cpp
  src/ui/console.cpp
)

target_include_directories(cyberpunk2077 PRIVATE include)
target_link_libraries(cyberpunk2077 PRIVATE cyberpunk_lib)

# TODO: target_link_libraries(cyberpunk2077 PRIVATE raylib)

# =============================================================================
# Tests (GTest)
# =============================================================================
if(CYBERPUNK_BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)

  add_executable(unit_tests
    tests/common/types_test.cpp
    tests/model/gameModel_test.cpp
    tests/controller/commandRegistry_test.cpp
  )

  target_include_directories(unit_tests PRIVATE
    include
    src/controller
  )
  target_link_libraries(unit_tests PRIVATE
    cyberpunk_lib
    GTest::gtest
    GTest::gtest_main
  )

  if(CYBERPUNK_ENABLE_COVERAGE)
    target_compile_options(unit_tests PRIVATE -O0 -g --coverage)
    target_compile_options(cyberpunk_lib PRIVATE -O0 -g --coverage)
    target_link_options(unit_tests PRIVATE --coverage)
  endif()

  include(GoogleTest)
  gtest_discover_tests(unit_tests)
endif()

# =============================================================================
# Benchmarks (Google Benchmark)
# =============================================================================
if(CYBERPUNK_BUILD_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)

  add_executable(benchmarks_run
    benchmarks/bench_gamemodel.cpp
          src/controller/HackCommand.cpp
          src/controller/HackCommand.h
  )

  target_include_directories(benchmarks_run PRIVATE include)
  target_link_libraries(benchmarks_run PRIVATE
    cyberpunk_lib
    benchmark::benchmark
    benchmark::benchmark_main
  )
endif()
