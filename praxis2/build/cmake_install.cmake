<<<<<<< HEAD
# Install script for directory: /mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2
=======
# Install script for directory: /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53

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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

<<<<<<< HEAD
# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

=======
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< HEAD
file(WRITE "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
