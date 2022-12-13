include(${CMAKE_SOURCE_DIR}/cmake/tools/CPM.cmake)

CPMAddPackage(
  NAME googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
