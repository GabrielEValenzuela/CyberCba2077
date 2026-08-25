add_executable(cybercba_app main.cpp
        ../game/CheckpointStack.cpp
        ../../include/cybercba/game/CheckpointStack.h
)

target_link_libraries(cybercba_app
    PRIVATE
        cybercba_core
        cybercba_raylib
)

cybercba_set_warnings(cybercba_app)
cybercba_enable_sanitizers(cybercba_app)
