###############################################################################
##
## CMakeLists file for unit test
##
###############################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

SET(SOURCES
  t-foo
  )

FOREACH(current ${SOURCES})
  ADD_EXECUTABLE(${current} ${current}.c)
  ADD_TEST(${current} ${current})
  TARGET_LINK_LIBRARIES(${current} myproof_measure m)
ENDFOREACH()

######################################################################################
