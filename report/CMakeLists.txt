CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(report)

SET(LATEX_COMPILER_FLAGS "-interaction=nonstopmode --enable-write18" CACHE STRING "Flags passed to latex.")
INCLUDE(UseLATEX.cmake)

ADD_LATEX_DOCUMENT(report.tex INPUTS introduction.tex static.tex profiler.tex DEFAULT_PDF)
