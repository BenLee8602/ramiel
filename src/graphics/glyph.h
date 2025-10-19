#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <ramiel/math.h>

namespace ramiel {

    class Glyph {
    public:
        Glyph() = default;
        Glyph(
            const std::vector<uint16_t>& contours,
            const std::vector<Vec2f>& points,
            const std::vector<bool>& onCurve
        );
        Glyph(
            std::vector<uint16_t>&& contours,
            std::vector<Vec2f>&& points,
            std::vector<bool>&& onCurve
        );

        Glyph(const Glyph&) = default;
        Glyph& operator=(const Glyph&) = default;

        Glyph(Glyph&&) = default;
        Glyph& operator=(Glyph&&) = default;

        Glyph operator+(const Glyph& r) const;

        float getWidth() const;
        void setWidth(float width);

        void transform(const Mat3x3f& tf);

        void draw(Vec2f pos, float size, Vec3f color) const;

    private:
        std::vector<uint16_t> contours;
        std::vector<Vec2f> points;
        std::vector<bool> onCurve;
        float width;
    };


    using Font = std::unordered_map<char, Glyph>;

    Font loadttf(std::string filename);

    void renderText(
        Vec2f pos,
        float size,
        Vec3f color,
        const Font& font,
        const std::string& text
    );

}

