###############################################################################
##
## CMakeLists file for unit test
##
###############################################################################

IF(UNIX)
  ADD_DEFINITIONS(-fplugin=${CMAKE_BINARY_DIR}/lib/libmyproof.so)
  ADD_DEFINITIONS(-O1)
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

SET(SOURCES
  t-test
  t-pragma
  t-pragma-error
  t-pragma-funcs
  t-partie1
  )

FOREACH(current ${SOURCES})
  ADD_EXECUTABLE(${current} ${current}.c)
  TARGET_LINK_LIBRARIES(${current} myproof_measure m)
  ADD_TEST(${current} ${current})
ENDFOREACH()

######################################################################################
