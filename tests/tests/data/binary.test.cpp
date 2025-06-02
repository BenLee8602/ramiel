#include <ramiel/test.h>
#include <ramiel/util.h>
#include <ramiel/data.h>
using namespace ramiel;
using namespace ramiel::test;


/*
attr1
node {
    attr2_1
    attr2_2
}
node {
    attr3
    node {
        attr4
    }
    node {
        attr5
    }
}
*/

RAMIEL_TEST_ADD(binary) {
    size_t count = 0;
    TempFile tmp;

    BinaryWriter writer(tmp.file());
    RAMIEL_TEST_ASSERT(writer.good());

    int32_t data1 = 2047;
    writer.writeAttr(&data1, sizeof(data1));

    writer.startNode();

    Vec3f data2_1 = { 3.1f, 4.1f, 5.9f };
    writer.writeAttr(&data2_1, sizeof(data2_1));

    std::vector<uint16_t> data2_2 = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
    writer.writeAttr(data2_2.data(), data2_2.size() * sizeof(uint16_t));

    writer.endNode();

    writer.startNode();

    float data3 = 1.618033f;
    writer.writeAttr(&data3, sizeof(data3));

    writer.startNode();

    Vec4u data4 = { 5, 10, 15, 20 };
    writer.writeAttr(&data4, sizeof(data4));

    writer.endNode();

    writer.startNode();

    std::vector<double> data5 = { 1.0, 1.414213, 1.732050, 2.0 };
    writer.writeAttr(data5.data(), data5.size() * sizeof(double));

    writer.done();

    using Mode = BinaryReader::Mode;
    BinaryReader reader(tmp.file());
    RAMIEL_TEST_ASSERT(reader.good());

    int32_t data1p;
    RAMIEL_TEST_ASSERT(reader.mode() == Mode::ATTR);
    RAMIEL_TEST_ASSERT(reader.attrSize() == sizeof(data1p));
    reader.readAttr(&data1p, sizeof(data1p));
    RAMIEL_TEST_ASSERT(data1p == data1);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_START);

    Vec3f data2_1p;
    RAMIEL_TEST_ASSERT(reader.next() == Mode::ATTR);
    RAMIEL_TEST_ASSERT(reader.attrSize() == sizeof(data2_1p));
    reader.readAttr(&data2_1p, sizeof(data2_1p));
    RAMIEL_TEST_ASSERT(data2_1p == data2_1);

    std::vector<uint16_t> data2_2p(data2_2.size());
    RAMIEL_TEST_ASSERT(reader.next() == Mode::ATTR);
    RAMIEL_TEST_ASSERT(reader.attrSize() == data2_2p.size() * sizeof(uint16_t));
    reader.readAttr(data2_2p.data(), data2_2p.size() * sizeof(uint16_t));
    RAMIEL_TEST_ASSERT(data2_2p == data2_2);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_END);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_START);

    float data3p;
    RAMIEL_TEST_ASSERT(reader.next() == Mode::ATTR);
    RAMIEL_TEST_ASSERT(reader.attrSize() == sizeof(float));
    reader.readAttr(&data3p, sizeof(data3p));
    RAMIEL_TEST_ASSERT(data3p == data3);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_START);

    Vec4u data4p;
    RAMIEL_TEST_ASSERT(reader.next() == Mode::ATTR);
    RAMIEL_TEST_ASSERT(reader.attrSize() == sizeof(data4p));
    reader.readAttr(&data4p, sizeof(data4p));
    RAMIEL_TEST_ASSERT(data4p == data4);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_END);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_START);

    std::vector<double> data5p(data5.size());
    RAMIEL_TEST_ASSERT(reader.next() == Mode::ATTR);
    RAMIEL_TEST_ASSERT(reader.attrSize() == data5p.size() * sizeof(double));
    reader.readAttr(data5p.data(), data5p.size() * sizeof(double));
    RAMIEL_TEST_ASSERT(data5p == data5);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_END);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::NODE_END);

    RAMIEL_TEST_ASSERT(reader.next() == Mode::ERROR);
    RAMIEL_TEST_ASSERT(!reader.good());
}
