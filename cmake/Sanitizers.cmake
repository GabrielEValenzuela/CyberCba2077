# Applies ASan+UBSan to a target when CYBERCBA_ENABLE_SANITIZERS is on (TAD-001 §30).
function(cybercba_enable_sanitizers target)
    if(CYBERCBA_ENABLE_SANITIZERS)
        if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            message(WARNING "Sanitizers requested but compiler is not Clang/GNU; skipping for ${target}")
            return()
        endif()
        target_compile_options(${target}
            PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )
        target_link_options(${target}
            PRIVATE
                -fsanitize=address,undefined
        )
    endif()
endfunction()
