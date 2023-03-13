
# Modules and dependencies

if (HPR_WITH_CONTAINERS)
    add_subdirectory(containers)
endif()

if (HPR_WITH_MATH)
    add_subdirectory(math)
endif()

if (HPR_WITH_IO)
    add_subdirectory(io)
endif()

if (HPR_WITH_MESH)
    add_subdirectory(mesh)
endif()

if (HPR_WITH_GEOMETRY)
    add_subdirectory(geometry)
endif()

if(HPR_WITH_CSG)
    include(${HPR_EXTERNAL_PATH}/occt.cmake)
    add_subdirectory(csg)
endif()

if(HPR_WITH_GPU)
    include(${HPR_EXTERNAL_PATH}/glad.cmake)
    include(${HPR_EXTERNAL_PATH}/glfw.cmake)
    include(${HPR_EXTERNAL_PATH}/stb.cmake)
    add_subdirectory(gpu)
endif()

if(HPR_WITH_PARALLEL)
    include(${HPR_EXTERNAL_PATH}/onetbb.cmake)
    add_subdirectory(parallel)
endif()
