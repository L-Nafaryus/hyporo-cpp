#pragma once

#define HPR_VERSION_MAJOR 0
#define HPR_VERSION_MINOR 10
#define HPR_VERSION_PATCH 0

#if HPR_WITH_CONTAINERS
#include <hpr/containers.hpp>
#endif

#if HPR_WITH_MATH
#include <hpr/math.hpp>
#endif

#if HPR_WITH_IO
#include <hpr/io.hpp>
#endif

#if HPR_WITH_GPU
#include <hpr/gpu.hpp>
#endif

#if HPR_WITH_MESH
#include <hpr/mesh.hpp>
#endif

#if HPR_WITH_CSG
#include <hpr/csg.hpp>
#endif

#if HPR_WITH_GEOMETRY
#include <hpr/geometry.hpp>
#endif

#if HPR_WITH_PARALLEL
#include <hpr/parallel.hpp>
#endif