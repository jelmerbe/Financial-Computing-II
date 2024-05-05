set(PROJECT_NAME "cfl")

include("${PROJECT_SOURCE_DIR}/CMake/lib.cmake")
target_link_libraries(${PROJECT_NAME} ${GSL_LIBRARIES})

if(${PROJECT_DOC} AND Doxygen_FOUND)
  set(DOXYGEN_TAGFILES ${STD_TAG})
  set(DOXYGEN_GENERATE_TAGFILE "${PROJECT_DOC_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.tag")
  include("${PROJECT_SOURCE_DIR}/CMake/dox.cmake")
endif()
