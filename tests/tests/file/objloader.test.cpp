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
