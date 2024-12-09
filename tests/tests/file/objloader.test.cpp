#include <ramiel/test.h>
#include <ramiel/file.h>
using namespace ramiel;

#ifdef ramiel_TEST_DATA_DIR
const std::string testDataDir = ramiel_TEST_DATA_DIR;
#else
const std::string testDataDir = ".";
#endif


RAMIEL_TEST_ADD(ObjLoader) {
    using FaceVtx = ObjData::FaceVtx;
    using Face = ObjData::Face;

    const std::vector<Face> fExpected = {
        { FaceVtx{ 1, 1, 0 }, { 3, 2, 0 }, { 7, 3, 0 } },
        { FaceVtx{ 1, 1, 0 }, { 7, 3, 0 }, { 5, 4, 0 } },
        { FaceVtx{ 5, 1, 0 }, { 7, 2, 0 }, { 8, 3, 0 } },
        { FaceVtx{ 5, 1, 0 }, { 8, 3, 0 }, { 6, 4, 0 } },
        { FaceVtx{ 6, 1, 0 }, { 8, 2, 0 }, { 4, 3, 0 } },
        { FaceVtx{ 6, 1, 0 }, { 4, 3, 0 }, { 2, 4, 0 } },
        { FaceVtx{ 2, 1, 0 }, { 4, 2, 0 }, { 3, 3, 0 } },
        { FaceVtx{ 2, 1, 0 }, { 3, 3, 0 }, { 1, 4, 0 } },
        { FaceVtx{ 3, 1, 0 }, { 4, 2, 0 }, { 8, 3, 0 } },
        { FaceVtx{ 8, 3, 0 }, { 7, 4, 0 }, { 3, 1, 0 } },
        { FaceVtx{ 5, 1, 0 }, { 6, 2, 0 }, { 2, 3, 0 } },
        { FaceVtx{ 2, 3, 0 }, { 1, 4, 0 }, { 5, 1, 0 } }
    };
    const std::vector<Vec3f> vExpected = {
        {  0.0f,  0.0f,  0.0f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f,  0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f,  0.5f }
    };
    const std::vector<Vec2f> vtExpected = {
        { 0.0f, 0.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    ObjData data = loadObj(testDataDir + "/cube_withquads.obj");

    RAMIEL_TEST_ASSERT(data.f == fExpected);
    RAMIEL_TEST_ASSERT(data.v == vExpected);
    RAMIEL_TEST_ASSERT(data.vt == vtExpected);

    VertexBuffer vertexBuffer = makeVertexBuffer(data);

    const std::vector<uint32_t> fbExpected = {
         0,  1,  2,
         0,  2,  3,
         4,  5,  6,
         4,  6,  7,
         8,  9, 10,
         8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 13,  6,
         6, 17, 16,
         4, 18, 19,
        19, 15,  4
    };
    const std::vector<float> vbExpected = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f
    };

    RAMIEL_TEST_ASSERT(vertexBuffer.f == fbExpected);
    RAMIEL_TEST_ASSERT(vertexBuffer.v == vbExpected);
}
