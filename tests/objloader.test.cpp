#include "catch2/catch2.hpp"
#include <objloader.h>
using namespace ramiel;


// objloader::splitv

TEST_CASE("splitv empty string", "[splitv]") {
    std::string in = "";
    std::vector<size_t> expected; // empty
    REQUIRE(objloader::splitv(in) == expected);
}

TEST_CASE("splitv v", "[splitv]") {
    std::string in = "123";
    std::vector<size_t> expected = { 123 };
    REQUIRE(objloader::splitv(in) == expected);
}

TEST_CASE("splitv v and vt", "[splitv]") {
    std::string in = "321/654";
    std::vector<size_t> expected = { 321, 654 };
    REQUIRE(objloader::splitv(in) == expected);
}

TEST_CASE("splitv v, vt, and vn", "[splitv]") {
    std::string in = "12/34/56";
    std::vector<size_t> expected = { 12, 34, 56 };
    REQUIRE(objloader::splitv(in) == expected);
}

TEST_CASE("splitv v and vn", "[splitv]") {
    std::string in = "21//34";
    std::vector<size_t> expected = { 21, 0, 34 };
    REQUIRE(objloader::splitv(in) == expected);
}


// objloader::count

TEST_CASE("counting non existent file throws", "[count][filenotfound]") {
    size_t v, vt, f;
    REQUIRE_THROWS(objloader::count("doesntexist.obj", v, vt, f));
}

TEST_CASE("count file", "[count]") {
    size_t v, vt, f;
    objloader::count(std::string(ramiel_TEST_DATA_DIR) + "/cube_tris.obj", v, vt, f);
    REQUIRE(v == 8);
    REQUIRE(vt == 4);
    REQUIRE(f == 12);
}

TEST_CASE("count file containing quads", "[count]") {
    size_t v, vt, f;
    objloader::count(std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj", v, vt, f);
    REQUIRE(v == 8);
    REQUIRE(vt == 4);
    REQUIRE(f == 12);
}


// objloader::load

TEST_CASE("loading non existent file throws", "[load][filenotfound]") {
    std::vector<Vec3f> v_pos;
    std::vector<Vec2f> v_txt;
    std::vector<Vec3u> tri;
    std::vector<Vec3u> tri_txt;
    REQUIRE_THROWS(objloader::load("doesntexist.obj", v_pos, v_txt, tri, tri_txt));
}

TEST_CASE("load file", "[load]") {
    std::vector<Vec3f> v_pos;
    std::vector<Vec2f> v_txt;
    std::vector<Vec3u> tri;
    std::vector<Vec3u> tri_txt;
    objloader::load(std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj", v_pos, v_txt, tri, tri_txt);

    std::vector<Vec3f> v_pos_expected = {
        { -0.5, -0.5, -0.5 },
        { -0.5, -0.5, 0.5 },
        { -0.5, 0.5, -0.5 },
        { -0.5, 0.5, 0.5 },
        { 0.5, -0.5, -0.5 },
        { 0.5, -0.5, 0.5 },
        { 0.5, 0.5, -0.5 },
        { 0.5, 0.5, 0.5 }
    };
    std::vector<Vec2f> v_txt_expected = {
        { 0, 0 },
        { 1, 0 },
        { 1, 1 },
        { 0, 1 }
    };
    std::vector<Vec3u> tri_expected = {
        { 0, 2, 6 },
        { 6, 4, 0 },
        { 4, 6, 7 },
        { 7, 5, 4 },
        { 5, 7, 3 },
        { 3, 1, 5 },
        { 1, 3, 2 },
        { 2, 0, 1 },
        { 2, 3, 7 },
        { 7, 6, 2 },
        { 4, 5, 1 },
        { 1, 0, 4 }
    };
    std::vector<Vec3u> tri_txt_expected = {
        { 0, 1, 2 },
        { 2, 3, 0 },
        { 0, 1, 2 },
        { 2, 3, 0 },
        { 0, 1, 2 },
        { 2, 3, 0 },
        { 0, 1, 2 },
        { 2, 3, 0 },
        { 0, 1, 2 },
        { 2, 3, 0 },
        { 0, 1, 2 },
        { 2, 3, 0 }
    };

    REQUIRE(v_pos == v_pos_expected);
    REQUIRE(v_txt == v_txt_expected);
    REQUIRE(tri == tri_expected);
    REQUIRE(tri_txt == tri_txt_expected);
}
