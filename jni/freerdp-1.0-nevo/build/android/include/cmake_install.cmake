# Install script for directory: /home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "./bin")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE FILE FILES
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/rail.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/update.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/primary.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/listener.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/peer.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/svc.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/pointer.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/dvc.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/wtypes.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/secondary.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/keyboard_scancode.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/altsec.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/settings.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/freerdp.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/extension.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/graphics.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/window.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/input.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/api.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/types.h"
    "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/constants.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/utils" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/channels" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/cache" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/gdi" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/rail" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/codec" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/plugins" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/locale" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/crypto" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp" TYPE DIRECTORY FILES "/home/itskewpie/android/workspace/FreeRDP/jni/freerdp-1.0-nevo-public/include/freerdp/auth" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

