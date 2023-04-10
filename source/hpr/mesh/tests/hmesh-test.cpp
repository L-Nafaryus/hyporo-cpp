#include <gtest/gtest.h>
#include <hpr/mesh/mesh.hpp>

TEST(hmeshTest, Mesh)
{
    hpr::mesh::Mesh mesh;
    mesh.addVertex(0, 0, 0);
    mesh.addVertex(1, 1, 1);
    mesh.addVertex(10, 0, 0);
    mesh.addVertex(0, 0, 0);
    mesh.addEdge(mesh.vertex(0), mesh.vertex(1));
    mesh.addEdge(mesh.vertex(1), mesh.vertex(2));
    mesh.addEdge(mesh.vertex(2), mesh.vertex(0));
    mesh.addEdge(mesh.vertex(0), mesh.vertex(3));
    mesh.addFace(mesh.edge(0), mesh.edge(1), mesh.edge(2));
    EXPECT_EQ(mesh.vertices().size(), 4);
    EXPECT_EQ(mesh.edges().size(), 4);
    EXPECT_EQ(mesh.faces().size(), 1);
    EXPECT_EQ(mesh.vertex(1)->refEdges().size(), 2);
    EXPECT_FALSE(mesh.edges().back()->isValid());
}
