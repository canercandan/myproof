###############################################################################
##
## CMakeLists file for unit test
##
###############################################################################

IF(UNIX)
  ADD_DEFINITIONS(-fplugin=${myproof_BINARY_DIR}/lib/lib${PROJECT_NAME}.so)
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCES
  t-test
  t-pragma
  )

FOREACH(current ${SOURCES})
  ADD_EXECUTABLE(${current} ${current}.c)
  ADD_TEST(${current} ${current})
  TARGET_LINK_LIBRARIES(${current} m)
  INSTALL(TARGETS ${current} RUNTIME DESTINATION share/${PROJECT_NAME}/test COMPONENT test)
ENDFOREACH()

######################################################################################
