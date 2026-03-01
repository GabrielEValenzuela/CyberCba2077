cmake_minimum_required(VERSION 3.30)
project(cyberpunk-cordoba-2077 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

# --- Options
option(CYBERPUNK_BUILD_TESTS "Build unit tests" ON)
option(CYBERPUNK_BUILD_BENCHMARKS "Build benchmarks" ON)

# -- Export compile commands for IDEs and tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Main app
add_executable(cyberpunk2077
  src/main.cpp
  src/model/GameModel.cpp
)

target_include_directories(cyberpunk2077 PRIVATE include)

# TODO: Add raylib linking here (depending on your template/integration)
# target_link_libraries(cyberpunk2077 PRIVATE raylib)

# --- Tests (GTest)
if(CYBERPUNK_BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)

  add_executable(unit_tests
    tests/test_gamemodel.cpp
    src/model/GameModel.cpp
  )

  target_include_directories(unit_tests PRIVATE include)
  target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(unit_tests)
endif()

# --- Benchmarks (Google Benchmark)
if(CYBERPUNK_BUILD_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)

  add_executable(benchmarks_run
    benchmarks/bench_gamemodel.cpp
    src/model/GameModel.cpp
  )

  target_include_directories(benchmarks_run PRIVATE include)
  target_link_libraries(benchmarks_run PRIVATE benchmark::benchmark benchmark::benchmark_main)
endif()
