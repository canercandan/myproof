###############################################################################
##
## CMakeLists file for unit test
##
###############################################################################

IF(UNIX)
  ADD_DEFINITIONS(-fplugin=${CMAKE_BINARY_DIR}/lib/libmyproof.so)
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../lib/debug/lib)

SET(SOURCES
  t-test
  t-pragma
  t-pragma-error
  t-pragma-funcs
  )

FOREACH(current ${SOURCES})
  ADD_EXECUTABLE(${current} ${current}.c)
  ADD_TEST(${current} ${current})
  TARGET_LINK_LIBRARIES(${current} m myproof)
ENDFOREACH()

######################################################################################
