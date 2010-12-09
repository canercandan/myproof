CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(report)

INCLUDE(UseLATEX.cmake)

# # Set GRI executable
# SET(GRI_COMPILE "/usr/bin/gri")
# # Set the location of data files
# SET(DATA_DIR data)
# # Set the location of the directory for image files
# SET(IMAGE_DIR graphics)
# # Get a list of gri files
# FILE(GLOB_RECURSE GRI_FILES "*.gri")
# FOREACH(file ${GRI_FILES})
#   GET_FILENAME_COMPONENT(basename "${file}" NAME_WE)
#   # Replace stings in gri file so data files can be found
#   FILE(READ
#     ${CMAKE_CURRENT_SOURCE_DIR}/${IMAGE_DIR}/${basename}.gri
#     file_contents
#     )
#   STRING(REPLACE "${DATA_DIR}" "${IMAGE_DIR}/${DATA_DIR}"
#     changed_file_contents ${file_contents}
#     )
#   FILE(WRITE
#     ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${basename}.gri
#     ${changed_file_contents}
#     )
#   # Command to run gri
#   IF(GRI_COMPILE)
#     ADD_CUSTOM_COMMAND(
#       OUTPUT
#       ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${basename}.eps
#       DEPENDS
#       ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${basename}.gri
#       ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${DATA_DIR}
#       COMMAND
#       ${GRI_COMPILE}
#       ARGS
#       -output
#       ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${basename}.eps
#       ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${basename}.gri
#       )
#   ENDIF(GRI_COMPILE)
#   # Make a list of all gri files (for ADD_LATEX_DOCUMENT depend)
#   SET(ALL_GRI_FILES ${ALL_GRI_FILES}
#     ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${basename}.eps
#     )
# ENDFOREACH(file)
# # Copy over all data files needed to generate gri graphs
# ADD_CUSTOM_COMMAND(
#   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${DATA_DIR}
#   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${IMAGE_DIR}/${DATA_DIR}
#   COMMAND ${CMAKE_COMMAND} -E copy_directory
#   ${CMAKE_CURRENT_SOURCE_DIR}/${IMAGE_DIR}/${DATA_DIR}
#   ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_DIR}/${DATA_DIR}
#   )

# ADD_LATEX_DOCUMENT(MyDoc.tex
#   IMAGE_DIRS ${IMAGE_DIR}
#   DEPENDS ${ALL_GRI_FILES}
#   )

ADD_LATEX_DOCUMENT(report.tex USE_GLOSSARY DEFAULT_PDF)
