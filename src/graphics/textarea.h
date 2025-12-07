#pragma once

#include <string>
#include <vector>

#include <ramiel/math.h>
#include "glyph.h"

namespace ramiel {

    class TextArea {
    public:
        TextArea();

        TextArea(const TextArea&) = default;
        TextArea& operator=(const TextArea&) = default;

        TextArea(TextArea&&) = default;
        TextArea& operator=(TextArea&&) = default;

        ~TextArea() = default;

        const std::string& getValue() const;
        size_t getLine() const;
        size_t getNLines() const;
        Vec2f getPos() const;
        Vec2f getSize() const;
        const Font* getFont() const;
        float getFontSize() const;
        Vec3f getFontColor() const;

        void setValue(const std::string& value);
        void setLine(size_t line);
        void setPos(Vec2f pos);
        void setSize(Vec2f size);
        void setFont(const Font* font);
        void setFontSize(float fontSize);
        void setFontColor(Vec3f fontColor);

        void goToBottom();

        void insert(const std::string& str);
        void insert(size_t i, const std::string& str);
        void erase(size_t i, size_t n);

        void render() const;

    private:
        std::string value;

        std::vector<size_t> lines;
        size_t line;

        Vec2f pos;
        Vec2f size;

        const Font* font;
        float fontSize;
        Vec3f fontColor;

        void calcLines(size_t c = 0);
    };

}

