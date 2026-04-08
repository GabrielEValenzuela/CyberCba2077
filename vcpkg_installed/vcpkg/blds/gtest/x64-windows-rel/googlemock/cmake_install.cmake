# Install script for directory: C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/src/v1.16.0-a2e0b328c1.clean/googlemock

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/src/v1.16.0-a2e0b328c1.clean/googlemock/include/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/lib/gmock.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/gmock.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/${CMAKE_INSTALL_CONFIG_NAME}/gmock.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/generated/gmock.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/src/v1.16.0-a2e0b328c1.clean/googlemock/include/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/manual-link" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/lib/gmock_main.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/gmock_main.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/manual-link" TYPE FILE OPTIONAL FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/bin/${CMAKE_INSTALL_CONFIG_NAME}/gmock_main.pdb")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "gmock" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/generated/gmock_main.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googletest/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Nico/Documents/GitHub/CyberCba2077/vcpkg_installed/vcpkg/blds/gtest/x64-windows-rel/googlemock/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
