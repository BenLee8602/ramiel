#include <vector>
#include <ramiel/test.h>
#include <ramiel/file.h>
using namespace ramiel;
#include <cassert>

RAMIEL_TEST_ADD(ImageLoad) {
    std::string filename;
    Vec2u res;
    std::vector<Vec3f> data;

    filename = std::string(ramiel_TEST_DATA_DIR) + "/doesnt_exist.png";
    RAMIEL_TEST_ASSERT(!loadImage(filename, res, data));

    filename = std::string(ramiel_TEST_DATA_DIR) + "/colortest.png";
    RAMIEL_TEST_ASSERT(loadImage(filename, res, data));
    RAMIEL_TEST_ASSERT((res == Vec2u{ 640, 480 }));
    RAMIEL_TEST_ASSERT(data.size() == res[X] * res[Y]);
}
