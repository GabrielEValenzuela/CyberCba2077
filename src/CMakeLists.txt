set(CYBERCBA_CORE_SOURCES
    core/Logger.cpp
    core/CreditsLoader.cpp
    game/GameStateMachine.cpp
    game/CampaignState.cpp
    game/Game.cpp
    combat/CombatSystem.cpp
    combat/GuardBehaviorStrategy.cpp
    missions/DialogueSystem.cpp
    missions/MissionSystem.cpp
    missions/PersuasionStrategy.cpp
    progression/SkillDefinition.cpp
    progression/ProgressionSystem.cpp
)

# PuzzleSystem depends on the custom structures, whose bodies are a student
# exercise left unimplemented in this pass. The object file compiles against
# the structures' public API but will not link into an executable until those
# bodies exist — kept as its own object library so cybercba_core /
# cybercba_tests can stay linkable in the meantime.
add_library(cybercba_core_puzzle OBJECT missions/PuzzleSystem.cpp)
target_include_directories(cybercba_core_puzzle PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(cybercba_core_puzzle PUBLIC cxx_std_17)
cybercba_set_warnings(cybercba_core_puzzle)
cybercba_enable_clang_tidy(cybercba_core_puzzle)

add_library(cybercba_core STATIC ${CYBERCBA_CORE_SOURCES})

target_include_directories(cybercba_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(cybercba_core PUBLIC cxx_std_17)

cybercba_set_warnings(cybercba_core)
cybercba_enable_clang_tidy(cybercba_core)
cybercba_enable_sanitizers(cybercba_core)

if(CYBERCBA_BUILD_APP)
    add_subdirectory(platform/raylib)
    add_subdirectory(app)
endif()
