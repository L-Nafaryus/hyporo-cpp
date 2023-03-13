
CPMAddPackage(
  NAME googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
  EXCLUDE_FROM_ALL ON
  OPTIONS "BUILD_GMOCK OFF" "INSTALL_GTEST OFF"
)
