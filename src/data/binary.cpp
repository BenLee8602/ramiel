#include "binary.h"
using namespace ramiel;

namespace {

    constexpr char modeError = static_cast<char>(BinaryReader::Mode::ERROR);
    constexpr char modeAttr = static_cast<char>(BinaryReader::Mode::ATTR);
    constexpr char modeNodeStart = static_cast<char>(BinaryReader::Mode::NODE_START);
    constexpr char modeNodeEnd = static_cast<char>(BinaryReader::Mode::NODE_END);

}

namespace ramiel {

    BinaryReader::BinaryReader(std::string filename)
        : m_file(filename, std::ios::binary)
        , m_mode(Mode::ERROR)
        , m_attrSize(0)
    {
        if (!m_file) return;
        next();
    }


    bool BinaryReader::good() const {
        return static_cast<bool>(m_file);
    }

    BinaryReader::Mode BinaryReader::mode() const {
        return m_mode;
    }


    BinaryReader::Mode BinaryReader::next() {
        if (!m_file) return Mode::ERROR;

        if (m_attrSize) m_file.seekg(m_attrSize, std::ios::cur);
        m_attrSize = 0;

        m_file.read(reinterpret_cast<char*>(&m_mode), 1);
        if (!m_file) m_mode = Mode::ERROR;

        if (m_mode == Mode::ATTR)
            m_file.read(reinterpret_cast<char*>(&m_attrSize), sizeof(m_attrSize));

        return m_mode;
    }


    uint32_t BinaryReader::attrSize() const {
        return m_attrSize;
    }

    uint32_t BinaryReader::readAttr(void* dest, uint32_t len) {
        if (!m_file || m_mode != Mode::ATTR) return 0;

        len = std::min(len, m_attrSize);
        m_attrSize -= len;
        if (!len) return 0;

        m_file.read(reinterpret_cast<char*>(dest), len);
        return len;
    }


    BinaryWriter::BinaryWriter(std::string filename)
        : m_file(filename, std::ios::binary)
        , m_nodeDepth(0)
    {}


    BinaryWriter::~BinaryWriter() {
        done();
    }


    bool BinaryWriter::good() const {
        return static_cast<bool>(m_file);
    }


    void BinaryWriter::startNode() {
        if (!m_file) return;
        m_file.write(&modeNodeStart, 1);
        m_nodeDepth++;
    }

    void BinaryWriter::endNode() {
        if (!m_file || !m_nodeDepth) return;
        m_file.write(&modeNodeEnd, 1);
        m_nodeDepth--;
    }

    void BinaryWriter::writeAttr(const void* src, uint32_t len) {
        if (!m_file) return;
        m_file.write(&modeAttr, 1);
        m_file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        m_file.write(reinterpret_cast<const char*>(src), len);
    }

    void BinaryWriter::done() {
        if (!m_file) return;
        for (m_nodeDepth; m_nodeDepth > 0; m_nodeDepth--) {
            m_file.write(&modeNodeEnd, 1);
        }
        m_file.close();
    }


    std::string readString(BinaryReader& file) {
        assert(file.good());
        std::string str;
        str.resize(file.attrSize());
        file.readAttr(str.data(), str.size());
        file.next();
        return str;
    }

    void writeString(BinaryWriter& file, const std::string& str) {
        assert(file.good());
        file.writeAttr(str.data(), str.size());
    }

}
