CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(report)

SET(LATEX_COMPILER_FLAGS "-interaction=nonstopmode --enable-write18" CACHE STRING "Flags passed to latex.")
INCLUDE(UseLATEX.cmake)

SET(SOURCE_INPUTS
  schema.tex
  introduction.tex
  pragma.tex
  static.tex
  dynamic.tex
  profiler.tex
  multithreading.tex
  critique.tex
  )

ADD_LATEX_DOCUMENT(${PROJECT_NAME}.tex INPUTS ${SOURCE_INPUTS} IMAGE_DIRS images DEFAULT_PDF)
