# Applies the project's warning baseline to a target.
# Only ever applied to first-party targets, never to raylib/GoogleTest.
function(cybercba_set_warnings target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Woverloaded-virtual
                -Wnull-dereference
                -Wdouble-promotion
                -Wformat=2
                -Wimplicit-fallthrough
                $<$<BOOL:${CYBERCBA_WARNINGS_AS_ERRORS}>:-Werror>
        )
    elseif(MSVC)
        target_compile_options(${target}
            PRIVATE
                /W4
                $<$<BOOL:${CYBERCBA_WARNINGS_AS_ERRORS}>:/WX>
        )
    endif()
endfunction()
