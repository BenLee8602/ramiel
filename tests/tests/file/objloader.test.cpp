#include <sstream>
#include <ramiel/test.h>
#include <ramiel/data.h>
using namespace ramiel;


RAMIEL_TEST_ADD(ObjloaderParseVec) {
    std::istringstream vec2("0.2 0.3");
    std::istringstream vec3("0.4 0.5 0.6");
    Vec2f vec2_expected = { 0.2f, 0.3f };
    Vec3f vec3_expected = { 0.4f, 0.5f, 0.6f };
    Vec2f vec2_actual = parseVec2(vec2);
    Vec3f vec3_actual = parseVec3(vec3);
    RAMIEL_TEST_ASSERT(vec2_expected == vec2_actual);
    RAMIEL_TEST_ASSERT(vec3_expected == vec3_actual);
}


RAMIEL_TEST_ADD(ObjloaderParsePolygonVertex) {
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

    RAMIEL_TEST_ASSERT(expected_v   == actual_v);
    RAMIEL_TEST_ASSERT(expected_vt  == actual_vt);
    RAMIEL_TEST_ASSERT(expected_vn  == actual_vn);
    RAMIEL_TEST_ASSERT(expected_vtn == actual_vtn);
}


RAMIEL_TEST_ADD(ObjloaderLoad) {
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
    RAMIEL_TEST_ASSERT(triangles == triangles_expected);

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
    RAMIEL_TEST_ASSERT(vertices.size() == vertices_expected.size());
    RAMIEL_TEST_ASSERT(std::equal(
        vertices.begin(), vertices.end(), vertices_expected.begin(),
        [](const Vertex& v1, const Vertex& v2) {
            return v1.pos == v2.pos && v1.txt == v2.txt;
        }
    ));

}
