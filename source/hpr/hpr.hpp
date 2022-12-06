#pragma once


namespace hpr
{
    /* Core */
    // containers
    // math
    // io

    /* Graphics */
    namespace gpu
    {
        // gpu
        // window_system
    }

    /* Mesh */
    namespace mesh
    {
        // mesh
    }

    /* CSG */
    namespace csg
    {
        // csg
    }
}

#include "containers.hpp"
#include "math.hpp"
#include "io.hpp"

#if WITH_GPU
#include "gpu.hpp"
#endif

#if WITH_WS
#include "window_system.hpp"
#endif

#if WITH_MESH
#include "mesh.hpp"
#endif

#if WITH_CSG
#include "csg.hpp"
#endif
