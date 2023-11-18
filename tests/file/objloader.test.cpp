#include <catch2/catch2.hpp>
#include <ramiel/file.h>
using namespace ramiel;


TEST_CASE("objloader", "[objloader]") {
    typedef Vertex_Mesh_T Vertex;
    std::vector<Vec3u> triangles;
    std::vector<Vertex> vertices;
    std::string filename = std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj";
    ObjLoader<Vertex>(filename.c_str(), vertices, triangles, true, true);

    std::vector<Vec3u> triangles_expected = {
        {  0,  1,  2 },
        {  0,  2,  3 },
        {  4,  5,  6 },
        {  4,  6,  7 },
        {  8,  9, 10 },
        {  8, 10, 11 },
        { 12, 13, 14 },
        { 12, 14, 15 },
        { 16, 13,  6 },
        {  6, 17, 16 },
        {  4, 18, 19 },
        { 19, 15,  4 }
    };
    REQUIRE(triangles == triangles_expected);

    std::vector<Vertex> vertices_expected = {
        { Vec3f{ -0.5, -0.5, -0.5 }, Vec2f{ 0, 0 } },
        { Vec3f{ -0.5,  0.5, -0.5 }, Vec2f{ 1, 0 } },
        { Vec3f{  0.5,  0.5, -0.5 }, Vec2f{ 1, 1 } },
        { Vec3f{  0.5, -0.5, -0.5 }, Vec2f{ 0, 1 } },
        { Vec3f{  0.5, -0.5, -0.5 }, Vec2f{ 0, 0 } },
        { Vec3f{  0.5,  0.5, -0.5 }, Vec2f{ 1, 0 } },
        { Vec3f{  0.5,  0.5,  0.5 }, Vec2f{ 1, 1 } },
        { Vec3f{  0.5, -0.5,  0.5 }, Vec2f{ 0, 1 } },
        { Vec3f{  0.5, -0.5,  0.5 }, Vec2f{ 0, 0 } },
        { Vec3f{  0.5,  0.5,  0.5 }, Vec2f{ 1, 0 } },
        { Vec3f{ -0.5,  0.5,  0.5 }, Vec2f{ 1, 1 } },
        { Vec3f{ -0.5, -0.5,  0.5 }, Vec2f{ 0, 1 } },
        { Vec3f{ -0.5, -0.5,  0.5 }, Vec2f{ 0, 0 } },
        { Vec3f{ -0.5,  0.5,  0.5 }, Vec2f{ 1, 0 } },
        { Vec3f{ -0.5,  0.5, -0.5 }, Vec2f{ 1, 1 } },
        { Vec3f{ -0.5, -0.5, -0.5 }, Vec2f{ 0, 1 } },
        { Vec3f{ -0.5,  0.5, -0.5 }, Vec2f{ 0, 0 } },
        { Vec3f{  0.5,  0.5, -0.5 }, Vec2f{ 0, 1 } },
        { Vec3f{  0.5, -0.5,  0.5 }, Vec2f{ 1, 0 } },
        { Vec3f{ -0.5, -0.5,  0.5 }, Vec2f{ 1, 1 } }
    };
    REQUIRE(vertices.size() == vertices_expected.size());
    REQUIRE(std::equal(
        vertices.begin(), vertices.end(), vertices_expected.begin(),
        [](const Vertex& v1, const Vertex& v2) {
            return v1.pos == v2.pos && v1.texture == v2.texture;
        }
    ));

}
