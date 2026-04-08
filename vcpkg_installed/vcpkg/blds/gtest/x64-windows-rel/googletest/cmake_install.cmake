# Install script for directory: C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/src/v1.16.0-a2e0b328c1.clean/googletest

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/pkgs/gtest_x64-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest/GTestTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest/GTestTargets.cmake"
         "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/CMakeFiles/Export/0c08b8e77dd885bfe55a19a9659d9fc1/GTestTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest/GTestTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest/GTestTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/CMakeFiles/Export/0c08b8e77dd885bfe55a19a9659d9fc1/GTestTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/CMakeFiles/Export/0c08b8e77dd885bfe55a19a9659d9fc1/GTestTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/GTest" TYPE FILE FILES
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/generated/GTestConfigVersion.cmake"
    "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/generated/GTestConfig.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/src/v1.16.0-a2e0b328c1.clean/googletest/include/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/lib/gtest.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/gtest.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/${CMAKE_INSTALL_CONFIG_NAME}/gtest.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/generated/gtest.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/src/v1.16.0-a2e0b328c1.clean/googletest/include/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/manual-link" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/lib/gtest_main.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/gtest_main.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/manual-link" TYPE FILE OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/${CMAKE_INSTALL_CONFIG_NAME}/gtest_main.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gtest" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/generated/gtest_main.pc")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
