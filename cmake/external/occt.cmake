
find_package(OpenCASCADE QUIET)

if (NOT OpenCASCADE_FOUND)
    CPMAddPackage(
        NAME OpenCASCADE
        GIT_REPOSITORY https://github.com/Open-Cascade-SAS/OCCT.git
        GIT_TAG V7_6_2
        DOWNLOAD_ONLY YES
    )

    if(OpenCASCADE_ADDED)
        # They are using CMAKE_SOURCE_DIR and CMAKE_BINARY_DIR for the project root, fix it
        file(READ ${OpenCASCADE_SOURCE_DIR}/CMakeLists.txt filedata_)
        string(FIND "${filedata_}" "CMAKE_SOURCE_DIR" need_patch)

        if(NOT ${need_patch} EQUAL -1)
            string(REPLACE "CMAKE_SOURCE_DIR" "OCCT_SOURCE_DIR" filedata_ "${filedata_}")
            string(REPLACE "CMAKE_BINARY_DIR" "OCCT_BINARY_DIR" filedata_ "${filedata_}")
            string(REPLACE "project (OCCT)" "" filedata_ "${filedata_}")
            string(PREPEND filedata_ "project(OCCT)\nset(OCCT_BINARY_DIR $\{_OCCT_BINARY_DIR\})\n")
        endif()
        file(WRITE ${OpenCASCADE_SOURCE_DIR}/CMakeLists.txt "${filedata_}")

        file(GLOB_RECURSE files_to_patch ${OpenCASCADE_SOURCE_DIR}/adm/cmake "occt_*")

        foreach(file_path ${files_to_patch})
            file(READ ${file_path} filedata_)
            string(REPLACE "CMAKE_SOURCE_DIR" "OCCT_SOURCE_DIR" filedata_ "${filedata_}")
            string(REPLACE "CMAKE_BINARY_DIR" "OCCT_BINARY_DIR" filedata_ "${filedata_}")
            file(WRITE ${file_path} "${filedata_}")
        endforeach()

        #project(OCCT)
        # find better way to pass build directory
        #set(_OCCT_BINARY_DIR ${OpenCASCADE_BINARY_DIR})
        set(INSTALL_DIR ${OpenCASCADE_BINARY_DIR} CACHE BOOL "" FORCE)

        set(USE_TK OFF CACHE BOOL "" FORCE)
        set(USE_FREETYPE OFF CACHE BOOL "" FORCE)
        set(USE_TCL OFF CACHE INTERNAL "" FORCE)

        set(BUILD_MODULE_Visualization OFF CACHE BOOL "" FORCE)
        set(BUILD_MODULE_ApplicationFramework OFF CACHE BOOL "" FORCE)
        set(BUILD_MODULE_Draw OFF CACHE BOOL "" FORCE)

        add_subdirectory(${OpenCASCADE_SOURCE_DIR})
    endif()
endif()

set(OpenCASCADE_LIBS
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

