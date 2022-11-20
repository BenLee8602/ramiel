#include "catch2/catch2.hpp"
#include <objloader.h>
using namespace ramiel;

#ifndef ramiel_TEST_DATA_DIR
#define ramiel_TEST_DATA_DIR "."
#endif


TEST_CASE("objloader", "[objloader]") {
    typedef Vertex_Mesh_T Vertex;
    std::vector<Vec3u> triangles;
    std::vector<Vertex> vertices;
    std::string filename = std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj";
    ObjLoader(filename, triangles, vertices, true, true);

    std::vector<Vec3u> triangles_expected = {
        { 0, 2, 6 },
        { 0, 6, 4 },
        { 4, 6, 7 },
        { 4, 7, 5 },
        { 5, 7, 3 },
        { 5, 3, 1 },
        { 1, 3, 2 },
        { 1, 2, 0 },
        { 2, 3, 7 },
        { 7, 6, 2 },
        { 4, 5, 1 },
        { 1, 0, 4 }
    };
    REQUIRE(triangles == triangles_expected);

    std::vector<Vertex> vertices_expected = {
        { { -0.5, -0.5, -0.5 }, { 0, 1 } },
        { { -0.5, -0.5,  0.5 }, { 1, 1 } },
        { { -0.5,  0.5, -0.5 }, { 1, 1 } },
        { { -0.5,  0.5,  0.5 }, { 1, 0 } },
        { {  0.5, -0.5, -0.5 }, { 0, 1 } },
        { {  0.5, -0.5,  0.5 }, { 1, 0 } },
        { {  0.5,  0.5, -0.5 }, { 0, 1 } },
        { {  0.5,  0.5,  0.5 }, { 1, 1 } }
    };
    REQUIRE(vertices.size() == vertices_expected.size());
    REQUIRE(std::equal(
        vertices.begin(), vertices.end(), vertices_expected.begin(),
        [](const Vertex& v1, const Vertex& v2) {
            return v1.pos == v2.pos && v1.texture == v2.texture;
        }
    ));

}
