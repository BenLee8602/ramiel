#include <cassert>
#include <type_traits>
#include <algorithm>
#include <fstream>
#include <list>
#include <array>
#include <functional>

#include "glyph.h"
#include "camera.h"
using namespace ramiel;

namespace {

    template<typename T>
    bool getBit(T num, uint8_t pos) {
        static_assert(std::is_integral_v<T>);
        return (num >> pos) & 1;
    }

    Vec2f matvec1(const Mat3x3f& tf, const Vec2f& v) {
        return sizeView<2>(matvec(tf, Vec3f{ v[X], v[Y], 1 }));
    }


    template<typename T>
    std::vector<T> concat(const std::vector<T>& v0, const std::vector<T>& v1) {
        std::vector<T> v;
        v.reserve(v0.size() + v1.size());
        v.insert(v.end(), v0.begin(), v0.end());
        v.insert(v.end(), v1.begin(), v1.end());
        return v;
    }


    template<typename T>
    T swapEndian(T in) {
        static_assert(std::is_arithmetic_v<T>);

        T out;
        auto ip = reinterpret_cast<uint8_t*>(&in);
        auto op = reinterpret_cast<uint8_t*>(&out) + sizeof(T) - 1;

        for (size_t i = 0; i < sizeof(T); i++) {
            *op-- = *ip++;
        }

        return out;
    }

    template<typename T>
    T readBinary(std::ifstream& file) {
        assert(file);
        T out;
        file.read(reinterpret_cast<char*>(&out), sizeof(T));
        return swapEndian(out);
    }

    float readFixed2Dot14(std::ifstream& file) {
        assert(file);
        int16_t num = readBinary<int16_t>(file);
        return static_cast<float>(num) / 16384.0f;
    }

    
    using TtfTables = std::unordered_map<std::string, uint32_t>;

    std::ifstream ttf;
    TtfTables tables;


    uint32_t readGlyphLoc(uint8_t size) {
        assert(ttf);
        assert(size == 2 || size == 4);
        if (size == 4) return readBinary<uint32_t>(ttf);
        return readBinary<uint16_t>(ttf) * 2;
    }


    Glyph loadGlyph(uint32_t glyphIdx);

    Glyph loadCompoundGlyph(uint32_t glyphIdx) {
        assert(ttf);
        Glyph g;

        while (true) {
            uint16_t flags = readBinary<uint16_t>(ttf);
            uint16_t glyphIdx = readBinary<uint16_t>(ttf);

            // relative offset not supported
            if (!getBit(flags, 1)) return Glyph();

            // load component glyph
            std::streampos filepos = ttf.tellg();
            Glyph cg = loadGlyph(glyphIdx);
            ttf.seekg(filepos);

            Mat3x3f tf = id<float, 3>();

            // load offset
            if (getBit(flags, 0)) { // 16-bit offsets
                tf[X][Z] = readBinary<int16_t>(ttf);
                tf[Y][Z] = readBinary<int16_t>(ttf);
            } else { // 8-bit offsets
                tf[X][Z] = readBinary<int8_t>(ttf);
                tf[Y][Z] = readBinary<int8_t>(ttf);
            }

            // load transformation
            if (getBit(flags, 3)) { // xscale == yscale
                float s = readFixed2Dot14(ttf);
                tf[X][X] = s;
                tf[Y][Y] = s;
            } else if (getBit(flags, 6)) { // xscale != yscale
                tf[X][X] = readFixed2Dot14(ttf);
                tf[Y][Y] = readFixed2Dot14(ttf);
            } else if (getBit(flags, 7)) { // 2x2 mat
                tf[X][X] = readFixed2Dot14(ttf);
                tf[X][Y] = readFixed2Dot14(ttf);
                tf[Y][X] = readFixed2Dot14(ttf);
                tf[Y][Y] = readFixed2Dot14(ttf);
            }

            cg.transform(tf);
            g = g + cg;

            if (!getBit(flags, 5)) break; // current component is last
        }

        return g;
    }


    Glyph loadGlyph(uint32_t glyphIdx) {
        assert(ttf);

        // get glyph location
        ttf.seekg(tables["head"] + 50);
        uint8_t glyphLocSize = readBinary<int16_t>(ttf) ? 4 : 2;

        ttf.seekg(tables["loca"] + glyphIdx * glyphLocSize);
        uint32_t glyphLoc = readGlyphLoc(glyphLocSize);
        uint32_t nextGlyphLoc = readGlyphLoc(glyphLocSize);
        if (glyphLoc == nextGlyphLoc) return Glyph();
        ttf.seekg(tables["glyf"] + glyphLoc);

        // glyph description
        int16_t nContours = readBinary<int16_t>(ttf);
        ttf.ignore(4 * sizeof(int16_t)); // glyph bounding box
        if (nContours <= 0) return loadCompoundGlyph(glyphIdx);

        // contour end indices
        std::vector<uint16_t> contours(nContours);
        for (uint16_t i = 0; i < nContours; i++) {
            contours[i] = readBinary<uint16_t>(ttf);
        }
        uint16_t nPoints = contours[contours.size() - 1] + 1;

        // glyph instructions
        ttf.ignore(readBinary<uint16_t>(ttf));

        // flags
        std::vector<uint8_t> flags(nPoints);
        std::vector<std::list<bool>> onCurve(nContours);
        uint16_t i = 0;
        for (uint16_t c = 0; c < nContours; c++) {
            for (i; i <= contours[c]; i++) {
                uint8_t flag = readBinary<uint8_t>(ttf);
                flags[i] = flag;
                onCurve[c].push_back(flag & 1);
                if (!((flag >> 3) & 1)) continue;

                uint8_t repeats = readBinary<uint8_t>(ttf);
                for (size_t j = 0; j < repeats; j++) {
                    flags[++i] = flag;
                    onCurve[c].push_back(flag & 1);
                }
            }
        }

        // points
        std::array<std::vector<std::list<float>>, 2> points;
        for (uint8_t a = X; a <= Y; a++) {
            points[a].resize(nContours);

            i = 0;
            int32_t v = 0;

            for (uint16_t c = 0; c < nContours; c++) {
                for (i; i <= contours[c]; i++) {
                    if (getBit(flags[i], 1 + a)) {
                        int16_t offset = readBinary<uint8_t>(ttf);
                        int16_t sign = getBit(flags[i], 4 + a) ? 1 : -1;
                        v += sign * offset;
                    } else if (!getBit(flags[i], 4 + a)) {
                        v += readBinary<int16_t>(ttf);
                    }

                    points[a][c].push_back(static_cast<float>(v));
                }
            }
        }

        // insert implied on-curve points
        for (uint16_t c = 0; c < nContours; c++) {
            auto i_crv = onCurve[c].begin();
            auto i_ptx = points[X][c].begin();
            auto i_pty = points[Y][c].begin();

            const auto j_crv = std::prev(onCurve[c].end());
            const auto j_ptx = std::prev(points[X][c].end());
            const auto j_pty = std::prev(points[Y][c].end());

            // ensure first and last points are on-curve
            if (!*i_crv && !*j_crv) {
                float x = (*i_ptx + *j_ptx) / 2.0f;
                float y = (*i_pty + *j_pty) / 2.0f;
                onCurve[c].push_front(true);
                points[X][c].push_front(x);
                points[Y][c].push_front(y);
                onCurve[c].push_back(true);
                points[X][c].push_back(x);
                points[Y][c].push_back(y);
            } else if (!*i_crv) {
                onCurve[c].push_front(true);
                points[X][c].push_front(*j_ptx);
                points[Y][c].push_front(*j_pty);
            } else if (!*j_crv) {
                onCurve[c].push_back(true);
                points[X][c].push_back(*i_ptx);
                points[Y][c].push_back(*i_pty);
            }

            // split all consecutive off-curve points
            while (i_crv != j_crv) {
                i_crv++;
                i_ptx++;
                i_pty++;

                if (*i_crv || *std::prev(i_crv)) continue;
                onCurve[c].insert(i_crv, true);
                points[X][c].insert(i_ptx, (*std::prev(i_ptx) + *i_ptx) / 2.0f);
                points[Y][c].insert(i_pty, (*std::prev(i_pty) + *i_pty) / 2.0f);
            }
        }

        // update contour ends
        i = 0;
        for (uint16_t c = 0; c < nContours; c++) {
            i += onCurve[c].size();
            contours[c] = i - 1;
        }
        nPoints = contours[contours.size() - 1] + 1;

        // flatten onCurve and points
        std::vector<bool> onCurve2(nPoints);
        std::vector<Vec2f> points2(nPoints);

        i = 0;
        for (uint16_t c = 0; c < nContours; c++) {
            auto i_crv = onCurve[c].begin();
            auto i_ptx = points[X][c].begin();
            auto i_pty = points[Y][c].begin();
            
            while (i_crv != onCurve[c].end()) {
                onCurve2[i] = *i_crv++;
                points2[i][X] = *i_ptx++;
                points2[i][Y] = *i_pty++;
                i++;
            }
        }

        return Glyph(
            std::move(contours),
            std::move(points2),
            std::move(onCurve2)
        );
    }


    using CmapFn = std::function<void(char, uint32_t)>;
    using CmapReader = std::function<void(CmapFn)>;

    void readCmap4(CmapFn fn) {
        assert(ttf);

        ttf.ignore(6);
        uint16_t nSeg = readBinary<uint16_t>(ttf) / 2;

        ttf.ignore(6);
        std::streampos endCode = ttf.tellg();

        ttf.ignore((nSeg + 1) * sizeof(uint16_t));
        std::streampos startCode = ttf.tellg();

        ttf.ignore(nSeg * sizeof(uint16_t));
        std::streampos idDelta = ttf.tellg();

        ttf.ignore(nSeg * sizeof(int16_t));
        std::streampos idRangeOffset = ttf.tellg();

        for (uint16_t i = 0; i < nSeg - 1; i++) {
            ttf.seekg(i * sizeof(uint16_t) + endCode);
            uint16_t end = readBinary<uint16_t>(ttf);

            ttf.seekg(i * sizeof(uint16_t) + startCode);
            uint16_t beg = readBinary<uint16_t>(ttf);

            if (end < 32) continue;
            if (beg > 126 || end == 0xffff) break;

            ttf.seekg(i * sizeof(int16_t) + idDelta);
            int16_t del = readBinary<int16_t>(ttf);

            ttf.seekg(i * sizeof(uint16_t) + idRangeOffset);
            uint16_t iro = readBinary<uint16_t>(ttf);
            if (iro) ttf.ignore(iro - sizeof(uint16_t));

            uint16_t c0 = std::max<uint16_t>(32, beg);
            uint16_t n = std::min<uint16_t>(end, 126);
            for (uint16_t c = c0; c <= n; c++) {
                if (iro == 0) {
                    fn(c, c + del);
                    continue;
                }

                uint16_t g = readBinary<uint16_t>(ttf) + del;
                std::streampos pos = ttf.tellg();
                fn(c, g);
                ttf.seekg(pos);
            }
        }
    }

    void readCmap12(CmapFn fn) {
        assert(ttf);

        ttf.ignore(12);
        uint32_t nCmapGroups = readBinary<uint32_t>(ttf);
        std::streampos groupsBeg = ttf.tellg();

        for (uint32_t i = 0; i < nCmapGroups; i++) {
            ttf.seekg(groupsBeg);
            ttf.ignore(i * (sizeof(uint32_t) * 3));

            uint32_t begCharCode = readBinary<uint32_t>(ttf);
            uint32_t endCharCode = readBinary<uint32_t>(ttf);
            uint32_t glyphIdx = readBinary<uint32_t>(ttf);
            
            uint32_t beg = std::max<uint32_t>(begCharCode, 32);
            uint32_t end = std::min<uint32_t>(endCharCode, 126);
            for (uint32_t c = beg; c <= end; c++, glyphIdx++) {
                fn(c, glyphIdx);
            }
        }
    }


    using IntsctTable = std::unordered_map<float, std::vector<float>>;


    float ceil2(float n) {
        return std::ceil(n + 0.5f) - 0.5f;
    }

    float getdx(const Vec2f& v0, const Vec2f& v1) {
        return (v1[X] - v0[X]) / (v1[Y] - v0[Y]);
    }

    float getx(const Vec2f& v, float dx, float y) {
        return v[X] + dx * (y - v[Y]);
    }


    void calcScanIntscts(IntsctTable& table, Vec2f p0, Vec2f p1) {
        // skip horizontal lines
        if (p0[Y] == p1[Y]) return;

        // ensure p0 is below p1
        bool down = p0[Y] > p1[Y];
        if (down) std::swap(p0, p1);
        assert(table.size() > static_cast<size_t>(p1[Y]));

        // get change in x between scanlines
        float dx = getdx(p0, p1);

        // add intersections to table
        for (float y = ceil2(p0[Y]); y <= p1[Y]; y += 1.0f) {
            float x = getx(p0, dx, y);
            table[y].emplace_back(down ? -x : x);
        }
    }


    void drawPixels(Vec3f color, float y, float x0, float x1) {
        // todo: proper clipping
        if (y < 0.0f || y > getRes()[Y]) return;

        assert(color >= 0.0f);
        assert(color <= 255.0f);

        x0 = ceil2(std::abs(x0));
        x1 = std::min<float>(std::abs(x1), getRes()[X]); // todo: here too
        assert(x1 < static_cast<float>(getRes()[X]));

        for (float x = ceil2(x0); x < x1; x += 1.0f) {
            size_t i = static_cast<size_t>(x) + static_cast<size_t>(y) * getRes()[X];
            getColorBuffer()[i] = color;
        }
    }

}

namespace ramiel {

    Glyph::Glyph(
        const std::vector<uint16_t>& contours,
        const std::vector<Vec2f>& points,
        const std::vector<bool>& onCurve
    )
        : contours(contours)
        , points(points)
        , onCurve(onCurve)
        , width(0.0f)
    {
        assert(contours.empty() || *(contours.end() - 1) == points.size() - 1);
        assert(points.size() == onCurve.size());
    }

    Glyph::Glyph(
        std::vector<uint16_t>&& contours,
        std::vector<Vec2f>&& points,
        std::vector<bool>&& onCurve
    )
        : contours(std::move(contours))
        , points(std::move(points))
        , onCurve(std::move(onCurve))
        , width(0.0f)
    {
        assert(contours.empty() || *(contours.end() - 1) == points.size() - 1);
        assert(points.size() == onCurve.size());
    }


    Glyph Glyph::operator+(const Glyph& r) const {
        const Glyph& l = *this;
        Glyph g;

        g.contours = concat(l.contours, r.contours);
        g.points = concat(l.points, r.points);
        g.onCurve = concat(l.onCurve , r.onCurve);
        
        for (size_t i = l.contours.size(); i < g.contours.size(); i++) {
            g.contours[i] += l.contours.size();
        }

        g.width = std::max(l.width, r.width);

        return g;
    }


    float Glyph::getWidth() const {
        return width;
    }

    void Glyph::setWidth(float width) {
        assert(width >= 0.0f);
        this->width = width;
    }


    void Glyph::transform(const Mat3x3f& tf) {
        for (auto& p : points) {
            p = matvec1(tf, p);
        }
    }


    void Glyph::draw(Vec2f pos, float size, Vec3f color) const {
        assert(pos > 0.0f);
        assert(size > 0.0f);
        assert(color >= 0.0f);
        assert(pos + size < getRes());

        // scanline-glyph intersections
        IntsctTable table;
        table.reserve(std::ceil(size));

        // glyph transformation matrix
        Mat3x3f tf = matmat(
            scale(Vec2f{ size, size }),
            translate(pos)
        );

        // for each contour
        uint16_t p0 = 0;
        for (const auto& c : contours) {
            // for each segment in contour
            for (uint16_t p = p0; p < c; p++) {
                // transform glyph control points
                Vec2f v0 = matvec1(tf, points[p]);
                Vec2f v1 = matvec1(tf, points[p + 1]);
                Vec2f v2 = matvec1(tf, points[p + 2]);
                
                // straight line
                if (onCurve[p + 1] == 1) {
                    calcScanIntscts(table, v0, v1);
                    continue;
                }

                // approx length of curve
                Vec2f d0 = v0 - v1;
                Vec2f d1 = v1 - v2;
                float dist = dot(d0, d0) + dot(d1, d1);
                
                // subdiv curve into n lines based on dist
                // then get scanline intersections for each line
                size_t nsteps = static_cast<size_t>(dist / 4.0f) + 2;
                float dt = 1.0f / static_cast<float>(nsteps);
                Vec2f prev = v0;
                for (size_t i = 1; i < nsteps; i++) {
                    float t = i * dt;
                    Vec2f cur = crvSolve(v0, v1, v2, t);
                    calcScanIntscts(table, prev, cur);
                    prev = cur;
                }
                calcScanIntscts(table, prev, v2);

                p++;
            }

            // connect start and end to close contour
            if (points[p0] != points[c]) {
                Vec2f v0 = matvec1(tf, points[p0]);
                Vec2f vc = matvec1(tf, points[c]);
                calcScanIntscts(table, vc, v0);
            }

            p0 = c + 1;
        }

        // for each scanline
        for (auto& [y, scan] : table) {
            assert(scan.size() > 1);

            // sort intersections ascending
            std::sort(
                scan.begin(),
                scan.end(),
                [](float i1, float i2) {
                    return std::abs(i1) < std::abs(i2);
                }
            );

            // draw pixels between each pair of intersections
            auto start = scan.begin();
            for (auto i = scan.begin() + 1; i < scan.end(); i++) {
                // edge case where one line ends and another begins
                // exactly on a scanline, resulting in 2 intersections.
                // ignore the duplicate
                if (std::abs(*i - *(i - 1)) < 1e-6f) continue;

                if (start == scan.end()) {
                    start = i;
                    continue;
                }

                drawPixels(color, y, *start, *i);
                start = scan.end();
            }
            assert(start == scan.end());
        }
    }


    Font loadttf(std::string filename) {
        class TtfSession {
        public:
            TtfSession() {
                assert(!ttf);
                assert(tables.empty());
            }
            ~TtfSession() {
                ttf = std::ifstream();
                tables = TtfTables();
            }
        };
        TtfSession session;

        ttf = std::ifstream(filename, std::ios::binary);
        if (!ttf) return Font();

        // get tables
        ttf.ignore(sizeof(uint32_t)); // scaler type
        uint16_t nTables = readBinary<uint16_t>(ttf);

        ttf.ignore(3 * sizeof(uint16_t));
        tables.reserve(nTables);
        for (uint16_t i = 0; i < nTables; i++) {
            std::string tag;
            tag.resize(4);

            ttf.read(tag.data(), sizeof(uint32_t)); // tag
            ttf.ignore(sizeof(uint32_t)); // checksum
            uint32_t offset = readBinary<uint32_t>(ttf); // offset
            ttf.ignore(sizeof(uint32_t)); // length

            tables.emplace(tag, offset);
        }

        // get char map with format 4 or 12
        ttf.seekg(tables["cmap"]);
        ttf.ignore(sizeof(uint16_t)); // version
        uint16_t nCmapEncTables = readBinary<uint16_t>(ttf);
        uint32_t cmapTableOffset = 0;
        for (uint16_t i = 0; i < nCmapEncTables; i++) {
            uint16_t platId = readBinary<uint16_t>(ttf);
            uint16_t encId = readBinary<uint16_t>(ttf);
            uint32_t offset = readBinary<uint32_t>(ttf);
            if (
                // format 12
                (platId == 0 && encId == 4) ||
                (platId == 3 && encId == 10) ||

                // format 4
                (platId == 0 && encId == 3) ||
                (platId == 3 && encId == 1)
            ) {
                cmapTableOffset = tables["cmap"] + offset;
                break;
            }
        }

        // unsupported cmap format
        if (!cmapTableOffset) return Font();
        ttf.seekg(cmapTableOffset);
        uint16_t cmapFormat = readBinary<uint16_t>(ttf);
        if (cmapFormat != 4 && cmapFormat != 12) return Font();

        // glyph scaling factor
        ttf.seekg(tables["head"] + 18);
        float glyphScale = 1.0f / readBinary<uint16_t>(ttf);
        Mat3x3f glyphScaleMat = scale(Vec2f{ glyphScale, glyphScale });

        // glyph widths
        ttf.seekg(tables["hhea"] + 34);
        uint32_t nGlyphWidths = readBinary<uint16_t>(ttf);

        Font font;
        font.reserve(127 - 32 + 1);

        // read { char code, glyph index } pairs from cmap
        CmapReader cmapReader = cmapFormat == 4 ? readCmap4 : readCmap12;
        ttf.seekg(cmapTableOffset);
        cmapReader([&](char c, uint32_t g) {
            // load glyph contours
            Glyph glyph = loadGlyph(g);

            // glyph width
            ttf.seekg(tables["hmtx"]);
            ttf.ignore(4 * std::min(g, nGlyphWidths - 1));
            glyph.setWidth(glyphScale * readBinary<uint16_t>(ttf));

            // glyph scale + offset
            ttf.ignore(2 * std::max<int32_t>(
                0, g - nGlyphWidths + 1));
            int16_t glyphOffset = readBinary<int16_t>(ttf);
            Mat3x3f glyphOffsetMat = translate(
                Vec2f{ static_cast<float>(glyphOffset), 0 });
            glyph.transform(matmat(glyphOffsetMat, glyphScaleMat));

            font.emplace(c, std::move(glyph));
        });

        return font;
    }


    void renderText(
        Vec2f pos,
        float size,
        Vec3f color,
        const Font& font,
        const std::string& text
    ) {
        for (char c : text) {
            auto g = font.find(c);
            if (g == font.end()) continue;
            g->second.draw(pos, size, color);
            pos[X] += g->second.getWidth() * size;
        }
    }

}

