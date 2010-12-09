######################################################################################
### 1) Set the application properties
######################################################################################

# Checks cmake version compatibility
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(install.cmake)

ENABLE_LANGUAGE(C)

PROJECT(myproof)

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
### 3) Now where we go ?
######################################################################################

ADD_SUBDIRECTORY(plugin)
ADD_SUBDIRECTORY(profiler)
ADD_SUBDIRECTORY(report)

######################################################################################
