
if(USE_SYSTEM_OCCT)
    find_package(OpenCASCADE REQUIRED)

    if(OpenCASCADE_FOUND)
        message(STATUS "OCCT: Found")
    else()
        message(FATAL "OCCT: Not Found")
    endif()

else()
    include(${CMAKE_SOURCE_DIR}/cmake/tools/CPM.cmake)

    CPMAddPackage(
            NAME occt
            GIT_REPOSITORY https://github.com/Open-Cascade-SAS/OCCT.git
            GIT_TAG V7_6_2
            DOWNLOAD_ONLY YES
    )

    if(occt_ADDED)
        # They are using CMAKE_SOURCE_DIR and CMAKE_BINARY_DIR for the project root, fix it
        file(READ ${occt_SOURCE_DIR}/CMakeLists.txt filedata_)
        string(FIND "${filedata_}" "CMAKE_SOURCE_DIR" need_patch)

        if(NOT ${need_patch} EQUAL -1)
            string(REPLACE "CMAKE_SOURCE_DIR" "OCCT_SOURCE_DIR" filedata_ "${filedata_}")
            string(REPLACE "CMAKE_BINARY_DIR" "OCCT_BINARY_DIR" filedata_ "${filedata_}")
            string(REPLACE "project (OCCT)" "" filedata_ "${filedata_}")
            string(PREPEND filedata_ "project(OCCT)\nset(OCCT_BINARY_DIR $\{_OCCT_BINARY_DIR\})\n")
        endif()
        file(WRITE ${occt_SOURCE_DIR}/CMakeLists.txt "${filedata_}")

        file(GLOB_RECURSE files_to_patch ${occt_SOURCE_DIR}/adm/cmake "occt_*")

        foreach(file_path ${files_to_patch})
            file(READ ${file_path} filedata_)
            string(REPLACE "CMAKE_SOURCE_DIR" "OCCT_SOURCE_DIR" filedata_ "${filedata_}")
            string(REPLACE "CMAKE_BINARY_DIR" "OCCT_BINARY_DIR" filedata_ "${filedata_}")
            file(WRITE ${file_path} "${filedata_}")
        endforeach()

        project(OCCT)
        # find better way to pass build directory
        set(_OCCT_BINARY_DIR ${occt_BINARY_DIR})
        set(INSTALL_DIR ${occt_BINARY_DIR} CACHE BOOL "" FORCE)

        set(USE_TK OFF CACHE BOOL "" FORCE)
        set(USE_FREETYPE OFF CACHE BOOL "" FORCE)
        set(USE_TCL OFF CACHE INTERNAL "" FORCE)

        set(BUILD_MODULE_Visualization OFF CACHE BOOL "" FORCE)
        set(BUILD_MODULE_ApplicationFramework OFF CACHE BOOL "" FORCE)
        set(BUILD_MODULE_Draw OFF CACHE BOOL "" FORCE)

        add_subdirectory(${occt_SOURCE_DIR})
    endif()
endif()

set(OCCT_LIBRARIES
        TKernel
        TKService
        TKV3d
        TKOpenGl
        TKBRep
        TKBool
        TKFillet
        TKGeomBase
        TKGeomAlgo
        TKG3d
        TKG2d
        TKTopAlgo
        TKPrim
        TKSTEP
        )
set(OCCT_INCLUDE_DIRS ${OpenCASCADE_INCLUDE_DIR})
