find_package(raylib CONFIG REQUIRED)

# vcpkg's raylib port exposes a pkg-config-style `raylib` target that does not
# propagate its own link dependencies (glfw3, X11) on Linux static builds —
# link them explicitly here rather than depend on raylib's INTERFACE_LINK_LIBRARIES.
if(UNIX AND NOT APPLE)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(X11 REQUIRED)
endif()

add_library(cybercba_raylib STATIC
    InputAdapter.cpp
    AssetStore.cpp
    AudioAdapter.cpp
    Renderer.cpp
        ../../game/CheckpointStack.cpp
        ../../../include/cybercba/game/CheckpointStack.h
)

target_include_directories(cybercba_raylib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cybercba_raylib
    PUBLIC
        cybercba_core
        raylib
        $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:glfw>
        $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:${X11_LIBRARIES}>
)

target_compile_features(cybercba_raylib PUBLIC cxx_std_17)

cybercba_set_warnings(cybercba_raylib)
