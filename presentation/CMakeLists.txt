CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(presentation)

#SET(LATEX_COMPILER_FLAGS "-interaction=nonstopmode --enable-write18" CACHE STRING "Flags passed to latex.")
INCLUDE(UseLATEX.cmake)

ADD_LATEX_DOCUMENT(${PROJECT_NAME}.tex IMAGE_DIRS images DEFAULT_PDF)
