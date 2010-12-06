######################################################################################
### 1) Set all needed source files for the project
######################################################################################

FILE(GLOB HDRS *.h)
#INSTALL(FILES ${HDRS} DESTINATION include/libmyproof COMPONENT headers)
FILE(GLOB SOURCES *.c)
SET(SAMPLE_SRCS ${SOURCES} PARENT_SCOPE)

######################################################################################
