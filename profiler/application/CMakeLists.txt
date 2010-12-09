SET(RESOURCES
  myproof.py
  )

FOREACH(file ${RESOURCES})
  EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/${file}
    ${profiler_BINARY_DIR}/${file}
    )
ENDFOREACH()
