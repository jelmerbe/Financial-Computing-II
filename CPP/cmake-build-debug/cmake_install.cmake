# Install script for directory: /Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/cfl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/setup/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Examples/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Homework1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Homework2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Homework3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Homework4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Homework5/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Session1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Session2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Session3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Session4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Session5/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Review1/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Review2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/ReviewExam/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/Exam/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/jelmerbennema/Documents/Oxford/MSc/Hilary/Financial Computing II/FC/CPP/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
