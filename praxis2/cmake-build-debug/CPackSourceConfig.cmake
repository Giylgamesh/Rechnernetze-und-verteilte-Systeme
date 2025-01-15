# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


<<<<<<< HEAD
set(CPACK_BINARY_DEB "OFF")
set(CPACK_BINARY_FREEBSD "OFF")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BINARY_NSIS "OFF")
=======
set(CPACK_BINARY_7Z "")
set(CPACK_BINARY_BUNDLE "")
set(CPACK_BINARY_CYGWIN "")
set(CPACK_BINARY_DEB "OFF")
set(CPACK_BINARY_DRAGNDROP "")
set(CPACK_BINARY_FREEBSD "OFF")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BINARY_NSIS "OFF")
set(CPACK_BINARY_NUGET "")
set(CPACK_BINARY_OSXX11 "")
set(CPACK_BINARY_PACKAGEMAKER "")
set(CPACK_BINARY_PRODUCTBUILD "")
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
set(CPACK_BINARY_RPM "OFF")
set(CPACK_BINARY_STGZ "ON")
set(CPACK_BINARY_TBZ2 "OFF")
set(CPACK_BINARY_TGZ "ON")
set(CPACK_BINARY_TXZ "OFF")
set(CPACK_BINARY_TZ "ON")
<<<<<<< HEAD
set(CPACK_BUILD_SOURCE_DIRS "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2;/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug")
=======
set(CPACK_BINARY_WIX "")
set(CPACK_BINARY_ZIP "")
set(CPACK_BUILD_SOURCE_DIRS "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2;/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug")
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENTS_ALL "")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
<<<<<<< HEAD
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.28/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "RN-Praxis built using CMake")
set(CPACK_GENERATOR "TGZ")
set(CPACK_IGNORE_FILES "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug;/\\..*\$")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALLED_DIRECTORIES "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2;/")
=======
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.16/Templates/CPack.GenericDescription.txt")
set(CPACK_GENERATOR "TGZ")
set(CPACK_IGNORE_FILES "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug;/\\..*\$")
set(CPACK_INSTALLED_DIRECTORIES "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2;/")
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
set(CPACK_INSTALL_CMAKE_PROJECTS "")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "")
set(CPACK_NSIS_DISPLAY_NAME "RN-Praxis 0.1.1")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "\$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "RN-Praxis 0.1.1")
<<<<<<< HEAD
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "/usr/bin/objcopy")
set(CPACK_OBJDUMP_EXECUTABLE "/usr/bin/objdump")
set(CPACK_OUTPUT_CONFIG_FILE "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.28/Templates/CPack.GenericDescription.txt")
=======
set(CPACK_OUTPUT_CONFIG_FILE "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.16/Templates/CPack.GenericDescription.txt")
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RN-Praxis built using CMake")
set(CPACK_PACKAGE_FILE_NAME "RN-Praxis-0.1.1-Source")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "RN-Praxis 0.1.1")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "RN-Praxis 0.1.1")
set(CPACK_PACKAGE_NAME "RN-Praxis")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
<<<<<<< HEAD
set(CPACK_READELF_EXECUTABLE "/usr/bin/readelf")
set(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake-3.28/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "/usr/share/cmake-3.28/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-3.28/Templates/CPack.GenericWelcome.txt")
set(CPACK_RPM_PACKAGE_SOURCES "ON")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug;/\\..*\$")
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2;/")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "RN-Praxis-0.1.1-Source")
set(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
set(CPACK_STRIP_FILES "")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_THREADS "1")
=======
set(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake-3.16/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "/usr/share/cmake-3.16/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-3.16/Templates/CPack.GenericWelcome.txt")
set(CPACK_RPM_PACKAGE_SOURCES "ON")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "")
set(CPACK_SOURCE_CYGWIN "")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug;/\\..*\$")
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2;/")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "RN-Praxis-0.1.1-Source")
set(CPACK_SOURCE_RPM "")
set(CPACK_SOURCE_TBZ2 "")
set(CPACK_SOURCE_TGZ "")
set(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
set(CPACK_SOURCE_TXZ "")
set(CPACK_SOURCE_TZ "")
set(CPACK_SOURCE_ZIP "")
set(CPACK_STRIP_FILES "")
set(CPACK_SYSTEM_NAME "Linux")
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
set(CPACK_TOPLEVEL_TAG "Linux-Source")
set(CPACK_VERBATIM_VARIABLES "YES")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
<<<<<<< HEAD
  set(CPACK_PROPERTIES_FILE "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug/CPackProperties.cmake")
=======
  set(CPACK_PROPERTIES_FILE "/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/cmake-build-debug/CPackProperties.cmake")
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
