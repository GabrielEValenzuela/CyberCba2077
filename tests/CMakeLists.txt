find_package(GTest CONFIG REQUIRED)

set(CYBERCBA_TEST_SOURCES
    core/CreditsLoaderTest.cpp
    states/GameStateMachineTest.cpp
    combat/CombatSystemTest.cpp
    combat/GuardBehaviorStrategyTest.cpp
    missions/DialogueSystemTest.cpp
    missions/MissionSystemTest.cpp
    missions/PersuasionStrategyTest.cpp
    progression/ProgressionSystemTest.cpp
        ../src/game/CheckpointStack.cpp
        ../include/cybercba/game/CheckpointStack.h
)

add_executable(cybercba_tests ${CYBERCBA_TEST_SOURCES}
        ../src/game/CheckpointStack.cpp
        ../include/cybercba/game/CheckpointStack.h
)

target_link_libraries(cybercba_tests
    PRIVATE
        cybercba_core
        GTest::gtest_main
)

cybercba_set_warnings(cybercba_tests)
cybercba_enable_sanitizers(cybercba_tests)

include(GoogleTest)
gtest_discover_tests(cybercba_tests)
