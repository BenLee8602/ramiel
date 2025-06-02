#pragma once

#include <fstream>
#include <string>

namespace ramiel {

    // todo: handle corrupt files gracefully
    class BinaryReader {
    public:
        enum class Mode : char {
            ERROR,
            ATTR,
            NODE_START,
            NODE_END
        };

        BinaryReader() = default;
        BinaryReader(std::string filename);

        BinaryReader(const BinaryReader&) = delete;
        BinaryReader& operator=(const BinaryReader&) = delete;

        BinaryReader(BinaryReader&&) = default;
        BinaryReader& operator=(BinaryReader&&) = default;

        bool good() const;
        Mode mode() const;

        Mode next();
        
        uint32_t attrSize() const;
        uint32_t readAttr(void* dest, uint32_t len);

    private:
        std::ifstream m_file;
        Mode m_mode;
        uint32_t m_attrSize;
    };


    class BinaryWriter {
    public:
        BinaryWriter() = default;
        BinaryWriter(std::string filename);

        BinaryWriter(const BinaryWriter&) = delete;
        BinaryWriter& operator=(const BinaryWriter&) = delete;

        BinaryWriter(BinaryWriter&&) = default;
        BinaryWriter& operator=(BinaryWriter&&) = default;

        ~BinaryWriter();

        bool good() const;

        void startNode();
        void endNode();
        void writeAttr(const void* src, uint32_t len);
        void done();

    private:
        std::ofstream m_file;
        size_t m_nodeDepth;
    };

}
