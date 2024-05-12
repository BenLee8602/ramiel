#include <vector>
#include <catch2/catch2.hpp>
#include <ramiel/file.h>
using namespace ramiel;


TEST_CASE("load image", "[image]") {
    std::string filename;
    Vec2u res;
    std::vector<Vec3f> data;

    filename = std::string(ramiel_TEST_DATA_DIR) + "/doesnt_exist.png";
    REQUIRE(!loadImage(filename, res, data));
    REQUIRE(res == Vec2u());
    REQUIRE(data.size() == 0);

    filename = std::string(ramiel_TEST_DATA_DIR) + "/colortest.png";
    REQUIRE(loadImage(filename, res, data));
    REQUIRE(res == Vec2u{ 640, 480 });
    REQUIRE(data.size() == res[X] * res[Y]);

}
