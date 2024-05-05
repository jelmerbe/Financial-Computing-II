file(GLOB PROJECT_SOURCE_FILES "Src/*.cpp")
file(GLOB INCLUDE_FILES "*.hpp" "Inline/*.hpp")
file(GLOB DOC_FILES "*.hpp")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES} ${INCLUDE_FILES}
        ../Exam/Src/forwardSwapRate.cpp
        ../Exam/Src/discountSwapLogLinInterp.cpp
        ../Exam/Src/downRebateUpOut.cpp
        ../Exam/Src/floorWithDeferredPayments.cpp)

target_link_libraries(${PROJECT_NAME} cfl)
target_link_libraries(${PROJECT_NAME} test_all)

file(MAKE_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}")    

configure_file (
  "${PROJECT_SOURCE_DIR}/CMake/Output.hpp.in"
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/Output.hpp"
  )

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
