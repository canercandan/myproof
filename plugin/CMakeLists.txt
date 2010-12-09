######################################################################################
### 1) Set the application properties
######################################################################################

# Checks cmake version compatibility
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(install.cmake)

ENABLE_LANGUAGE(C)

PROJECT(plugin)

SET(PROJECT_VERSION_MAJOR 1)
SET(PROJECT_VERSION_MINOR 0)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

######################################################################################


######################################################################################
### 2) Include useful features
######################################################################################

# INCLUDE(FindDoxygen)
# INCLUDE(FindPkgConfig)

######################################################################################


######################################################################################
### 3) Include header files path
######################################################################################

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

######################################################################################


######################################################################################
### 4) Set compiler definitions
######################################################################################

IF(UNIX)
  EXECUTE_PROCESS(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=plugin
    OUTPUT_VARIABLE PLUGIN_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

  INCLUDE_DIRECTORIES(${PLUGIN_DIR}/include)

  # enable warnings
  ADD_DEFINITIONS( -Wall -W ) #-Wextra )
  ADD_DEFINITIONS( -O2 )
  ADD_DEFINITIONS( -fPIC )
ENDIF()

######################################################################################


######################################################################################
### 5) Prepare some variables for CMAKE usage
######################################################################################

SET(SAMPLE_SRCS)

######################################################################################


######################################################################################
### 6) Now where we go ?
######################################################################################

ADD_SUBDIRECTORY(measure)
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(doc)

######################################################################################


######################################################################################
### 7) Create executable, link libraries and prepare target
######################################################################################

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
ADD_LIBRARY(myproof SHARED ${SAMPLE_SRCS})

######################################################################################
