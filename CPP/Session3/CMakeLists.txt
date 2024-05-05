set(PROJECT_NAME "Session3")

include("${PROJECT_SOURCE_DIR}/CMake/exe.cmake")

if(${PROJECT_DOC} AND Doxygen_FOUND)
set(DOXYGEN_TAGFILES "${CFL_TAG};${STD_TAG}")
include("${PROJECT_SOURCE_DIR}/CMake/dox.cmake")
endif()
