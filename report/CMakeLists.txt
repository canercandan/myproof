CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(report)

SET(LATEX_COMPILER_FLAGS "-interaction=nonstopmode --enable-write18" CACHE STRING "Flags passed to latex.")
INCLUDE(UseLATEX.cmake)

SET(INPUTS
  introduction.tex
  static.tex
  profiler.tex
  multithreading.tex
  )

ADD_LATEX_DOCUMENT(${PROJECT_NAME}.tex INPUTS ${INPUTS} IMAGE_DIRS images DEFAULT_PDF)
