# Install script for directory: C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/src/v1.9.1-3e0063176b.clean/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/pkgs/benchmark_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/benchmark.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/benchmark.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/benchmark_main.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/benchmark_main.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/src/v1.9.1-3e0063176b.clean/include/benchmark"
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/include/benchmark"
    FILES_MATCHING REGEX "/[^/]*\\.[^/]*h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark" TYPE FILE FILES
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/benchmarkConfig.cmake"
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/benchmarkConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/benchmark.pc"
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/benchmark_main.pc"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets.cmake"
         "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/CMakeFiles/Export/d56d27b88fbb159d81f220d5e4f5f598/benchmarkTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark/benchmarkTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/CMakeFiles/Export/d56d27b88fbb159d81f220d5e4f5f598/benchmarkTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/benchmark" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/CMakeFiles/Export/d56d27b88fbb159d81f220d5e4f5f598/benchmarkTargets-debug.cmake")
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/benchmark/x64-windows-dbg/src/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
