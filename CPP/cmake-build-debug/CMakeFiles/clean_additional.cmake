# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "doc/Exam/html"
  "doc/Examples/html"
  "doc/Homework1/html"
  "doc/Homework2/html"
  "doc/Homework3/html"
  "doc/Homework4/html"
  "doc/Homework5/html"
  "doc/Review1/html"
  "doc/Review2/html"
  "doc/ReviewExam/html"
  "doc/Session1/html"
  "doc/Session2/html"
  "doc/Session3/html"
  "doc/Session4/html"
  "doc/Session5/html"
  "doc/cfl/html"
  "doc/setup/html"
  "doc/test_all/html"
  )
endif()
