
CPMAddPackage(
    NAME onetbb
    GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
    GIT_TAG v2021.8.0
    EXCLUDE_FROM_ALL ON
    OPTIONS "TBB_INSTALL ON" "TBB_TEST OFF" "TBB_BENCH OFF" "BUILD_SHARED_LIBS ON"
)
