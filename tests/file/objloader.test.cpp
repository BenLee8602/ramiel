#include <catch2/catch2.hpp>
#include <ramiel/file.h>
using namespace ramiel;


TEST_CASE("parse vec", "[objloader]") {
    std::istringstream vec2("0.2 0.3");
    std::istringstream vec3("0.4 0.5 0.6");
    Vec2f vec2_expected = { 0.2f, 0.3f };
    Vec3f vec3_expected = { 0.4f, 0.5f, 0.6f };
    Vec2f vec2_actual = parseVec2(vec2);
    Vec3f vec3_actual = parseVec3(vec3);
    REQUIRE(vec2_expected == vec2_actual);
    REQUIRE(vec3_expected == vec3_actual);
}


TEST_CASE("parse polygon vertex", "[objloader]") {
    std::string str_v   = "1";
    std::string str_vt  = "2/3";
    std::string str_vn  = "4//5";
    std::string str_vtn = "6/7/8";

    Vec3u expected_v   = { 1, 0, 0 };
    Vec3u expected_vt  = { 2, 3, 0 };
    Vec3u expected_vn  = { 4, 0, 5 };
    Vec3u expected_vtn = { 6, 7, 8 };

    Vec3u actual_v   = parsePolygonVertex(str_v);
    Vec3u actual_vt  = parsePolygonVertex(str_vt);
    Vec3u actual_vn  = parsePolygonVertex(str_vn);
    Vec3u actual_vtn = parsePolygonVertex(str_vtn);

    REQUIRE(expected_v   == actual_v);
    REQUIRE(expected_vt  == actual_vt);
    REQUIRE(expected_vn  == actual_vn);
    REQUIRE(expected_vtn == actual_vtn);
}


TEST_CASE("load obj", "[objloader]") {
    typedef MeshVertexT Vertex;
    std::vector<Vec3u> triangles;
    std::vector<Vertex> vertices;
    std::string filename = std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj";
    getObj<Vertex>(filename.c_str(), vertices, triangles);

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
            return v1.pos == v2.pos && v1.txt == v2.txt;
        }
    ));

}
