######################################################################################
### 1) Set the application properties
######################################################################################

IF(UNIX)
  SET(CMAKE_C_COMPILER "gcc-4.5")
  SET(CMAKE_CXX_COMPILER "g++-4.5")
ENDIF()

ENABLE_LANGUAGE(C)

# Checks cmake version compatibility
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(myproof)

SET(PROJECT_VERSION_MAJOR 1)
SET(PROJECT_VERSION_MINOR 0)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

######################################################################################


######################################################################################
### 2) Include useful features
######################################################################################

INCLUDE(FindDoxygen)
INCLUDE(FindPkgConfig)

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
  INCLUDE_DIRECTORIES(/usr/lib/gcc/i686-linux-gnu/4.5.1/plugin/include) # will be replaced by `${CMAKE_C_COMPILER} -print-file-name=plugin`

  # enable warnings
  ADD_DEFINITIONS( -Wall -W -Wextra )
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
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SAMPLE_SRCS})
#INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib COMPONENT libraries)

######################################################################################


######################################################################################
### 8) Install pkg-config config file
######################################################################################

#INSTALL(FILES ${PROJECT_NAME}.pc DESTINATION lib/pkgconfig COMPONENT headers)

######################################################################################


######################################################################################
### 9) Include packaging
######################################################################################

#INCLUDE(Packaging.cmake)

######################################################################################
