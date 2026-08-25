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
option(CYBERPUNK_ENABLE_COVERAGE "Enable coverage flags" ON)
option(CYBERPUNK_BUILD_GAME "Build the raylib game when raylib is available" ON)

# -- Export compile commands for IDEs and tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Domain model shared by the game, tests and benchmarks.
add_library(cyberpunk_model STATIC
  src/model/GameModel.cpp
        include/progress_tracking/IAchievementCondition.cpp
        include/progress_tracking/IAchievementCondition.hpp
        include/progress_tracking/AchievementTracker.cpp
        include/progress_tracking/AchievementTracker.hpp
)

target_include_directories(cyberpunk_model PUBLIC include)

add_library(cyber_cba_core STATIC
  src/domain/GameSession.cpp
  src/domain/Campaign.cpp
  src/domain/QueueChallenge.cpp
  src/application/GameController.cpp
  src/application/WorldLayout.cpp
  src/infrastructure/SaveService.cpp
  src/infrastructure/CreditsLoader.cpp
  src/domain/MissionGraph.cpp
  src/domain/Dialogue.cpp
  src/domain/Evidence.cpp
  src/domain/Encounter.cpp
  src/domain/PrologueContent.cpp
  src/domain/EncounterContent.cpp
  src/domain/CharacterModule.cpp
  src/domain/characters/LumaCharacter.cpp
  src/domain/characters/SistemaCharacter.cpp
  src/domain/characters/VozFalsaMaggaCharacter.cpp
  src/domain/characters/OrdenFalsaCharacter.cpp
  src/domain/characters/ReleInterferenciaCharacter.cpp
  src/domain/characters/DronSeguridadCharacter.cpp
  src/domain/characters/SistemaVigilanciaCharacter.cpp
  src/domain/characters/PatrullaSeguridadCharacter.cpp
  src/domain/characters/TomoCharacter.cpp
  src/domain/characters/PrologueCharacters.cpp
        include/progress_tracking/IAchievementCondition.cpp
        include/progress_tracking/IAchievementCondition.hpp
        include/progress_tracking/AchievementTracker.cpp
        include/progress_tracking/AchievementTracker.hpp
)
target_include_directories(cyber_cba_core PUBLIC include)
target_link_libraries(cyber_cba_core PUBLIC cyberpunk_model)

# --- Cyber CBA game: powered by raylib. The domain/test targets remain usable
# in environments that intentionally do not provide graphical dependencies.
if(CYBERPUNK_BUILD_GAME)
find_package(raylib CONFIG QUIET)
find_package(glfw3 CONFIG QUIET)
if(raylib_FOUND)

add_executable(cyber_cba
  src/main_ui.cpp
  src/app/GameApp.cpp
  src/config/GameConfig.cpp
  src/ui/InputRouter.cpp
  src/ui/SceneStack.cpp
  src/ui/AudioService.cpp
  src/ui/AssetStore.cpp
  src/ui/widgets/DevOverlay.cpp
  src/ui/scenes/SplashScene.cpp
  src/ui/scenes/MainMenuScene.cpp
  src/ui/scenes/MapScene.cpp
  src/ui/scenes/HUDScene.cpp
  src/ui/scenes/InventoryScene.cpp
  src/ui/widgets/NlmButton.cpp
  src/ui/widgets/NlmPanel.cpp
  src/ui/widgets/NlmText.cpp
  src/ui/widgets/EvidencePanel.cpp
  src/ui/widgets/TextWrap.cpp
  src/ui/widgets/StoryPanel.cpp
  src/ui/widgets/ChoicePanel.cpp
  src/ui/widgets/ActionMenu.cpp
  src/ui/widgets/NodeMapView.cpp
  src/ui/widgets/StatusPanel.cpp
  src/ui/widgets/ParticipantPanel.cpp
  src/ui/widgets/TurnOrderPanel.cpp
  src/ui/widgets/EnemyIntentPanel.cpp
  src/ui/widgets/DialoguePanel.cpp
  src/ui/widgets/TransmissionPanel.cpp
  src/ui/widgets/MissionResultsView.cpp
  src/ui/widgets/CurrentObjectivePanel.cpp
  src/ui/widgets/PortraitFrame.cpp
  src/ui/widgets/InteractionPanel.cpp
)

set_target_properties(cyber_cba PROPERTIES OUTPUT_NAME "cyber-cba")
target_include_directories(cyber_cba PRIVATE include src)
target_link_libraries(cyber_cba PRIVATE cyber_cba_core raylib glfw)
target_compile_features(cyber_cba PRIVATE cxx_std_17)
target_compile_options(cyber_cba PRIVATE -Wall -Wextra)
configure_file(config/game.cfg "${CMAKE_CURRENT_BINARY_DIR}/game.cfg" COPYONLY)
configure_file(config/development.cfg "${CMAKE_CURRENT_BINARY_DIR}/development.cfg" COPYONLY)
target_compile_definitions(cyber_cba PRIVATE CYBER_CBA_DEFAULT_CONFIG_PATH="${CMAKE_CURRENT_BINARY_DIR}/game.cfg")
else()
  message(WARNING "raylib was not found; cyber-cba is not built. Install dependencies through vcpkg to enable it.")
endif()
endif()

# --- Tests (GTest)
if(CYBERPUNK_BUILD_TESTS)
  find_package(GTest CONFIG QUIET)
  if(GTest_FOUND)

  add_executable(unit_tests
    tests/test_gamemodel.cpp
    tests/test_vertical_slice.cpp
    tests/test_world_layout.cpp
    tests/test_mission_graph.cpp
    tests/test_dialogue.cpp
    tests/test_evidence.cpp
    tests/test_encounter.cpp
    tests/test_prologue_content.cpp
    tests/test_encounter_content.cpp
    tests/test_character_module.cpp
    tests/test_credits_loader.cpp
          include/progress_tracking/IAchievementCondition.cpp
          include/progress_tracking/IAchievementCondition.hpp
          include/progress_tracking/AchievementTracker.cpp
          include/progress_tracking/AchievementTracker.hpp
  )

  target_include_directories(unit_tests PRIVATE include)
  target_link_libraries(unit_tests PRIVATE cyber_cba_core GTest::gtest GTest::gtest_main)

  if(CYBERPUNK_ENABLE_COVERAGE)
    add_compile_options(-O0 -g --coverage)
    add_link_options(--coverage)
  endif()

  include(GoogleTest)
  gtest_discover_tests(unit_tests)
  else()
    message(WARNING "GTest was not found; unit_tests is not built. Install dependencies through vcpkg to enable it.")
  endif()
endif()

# --- Benchmarks (Google Benchmark)
if(CYBERPUNK_BUILD_BENCHMARKS)
  find_package(benchmark CONFIG QUIET)
  if(benchmark_FOUND)

  add_executable(benchmarks_run
    benchmarks/bench_gamemodel.cpp
          include/progress_tracking/IAchievementCondition.cpp
          include/progress_tracking/IAchievementCondition.hpp
          include/progress_tracking/AchievementTracker.cpp
          include/progress_tracking/AchievementTracker.hpp
  )

  target_include_directories(benchmarks_run PRIVATE include)
  target_link_libraries(benchmarks_run PRIVATE cyberpunk_model benchmark::benchmark benchmark::benchmark_main)
  else()
    message(WARNING "Google Benchmark was not found; benchmarks_run is not built. Install dependencies through vcpkg to enable it.")
  endif()
endif()
