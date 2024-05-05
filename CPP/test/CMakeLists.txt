set(PROJECT_NAME "test_all")

include("${PROJECT_SOURCE_DIR}/CMake/lib.cmake")
target_link_libraries(${PROJECT_NAME} cfl)

if(${PROJECT_DOC} AND Doxygen_FOUND)
set(DOXYGEN_TAGFILES "${CFL_TAG};${STD_TAG}")
include("${PROJECT_SOURCE_DIR}/CMake/dox.cmake")
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/test/Output.hpp.in"
  "${PROJECT_SOURCE_DIR}/test/Output.hpp"
  )
  
