######################################################################################
### 1) Set all needed source files for the project
######################################################################################

FILE(GLOB HDRS *.h)
#INSTALL(FILES ${HDRS} DESTINATION include/${PROJECT_NAME} COMPONENT headers)
FILE(GLOB SOURCES *.c)
SET(SAMPLE_SRCS ${SOURCES} PARENT_SCOPE)

######################################################################################
