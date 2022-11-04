#include <gtest/gtest.h>
#include "../mesh.hpp"

TEST(hmeshTest, Mesh)
{
    hpr::mesh::Mesh mesh;
    mesh.addVertex(1, 2, 3);
    mesh.addVertex(1, 3, 3);
    mesh.addEdge(mesh.vertex(0), mesh.vertex(1));
}
