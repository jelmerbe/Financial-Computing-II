cmake_minimum_required (VERSION 3.15)

project(FC VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall")

# enable testing
# include(CTest)

# set to YES to test for memory leaks, etc.
set (PROJECT_SANITIZE NO)
if(${PROJECT_SANITIZE}) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(OUTPUT_DIR "${PROJECT_BINARY_DIR}/output")
include_directories(${CMAKE_SOURCE_DIR})

set (PROJECT_DOC YES)
if(${PROJECT_DOC})
  find_package(Doxygen REQUIRED dot)
  if(${DOXYGEN_FOUND})
    set(DOXYGEN_EXTERNAL_GROUPS NO) 
    set(DOXYGEN_USE_MATHJAX YES)
    set(PROJECT_DOC_DIR "${PROJECT_BINARY_DIR}/doc")
    set(STD_TAG "${PROJECT_SOURCE_DIR}/CMake/cppreference-doxygen-web.tag.xml=http://en.cppreference.com/w/")
    set(CFL_TAG "${PROJECT_DOC_DIR}/cfl/cfl.tag=../../cfl/html")
  endif()
endif()

# GSL
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

# messages for testing
message("PROGRAMS AND LIBRARIES:")
message("CMake program: " ${CMAKE_MAKE_PROGRAM})
message("C++ compiler: " ${CMAKE_CXX_COMPILER})
message("Doxygen: " ${DOXYGEN_EXECUTABLE})
message("Doxygen dot: " ${DOXYGEN_DOT_EXECUTABLE})
message("pkg-config: " ${PKG_CONFIG_EXECUTABLE})
message("GSL include dir: " ${GSL_INCLUDE_DIRS})
message("GSL library: " ${GSL_LIBRARY})
message("BLAS library: " ${GSL_CBLAS_LIBRARY})

# write your Student ID in ""
set (STUDENT_ID "1079868")

add_subdirectory(cfl)
add_subdirectory(test)
add_subdirectory(setup)

# add subdirectories below, when you need them
add_subdirectory(Examples)

add_subdirectory(Homework1)
add_subdirectory(Homework2)
add_subdirectory(Homework3)
add_subdirectory(Homework4)
add_subdirectory(Homework5)

add_subdirectory(Session1)
add_subdirectory(Session2)
add_subdirectory(Session3)
add_subdirectory(Session4)
add_subdirectory(Session5)

add_subdirectory(Review1)
add_subdirectory(Review2)
# add_subdirectory(Review3)

add_subdirectory(ReviewExam)

add_subdirectory(Exam)
