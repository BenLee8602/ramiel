#include <cassert>
#include <algorithm>

#include "textarea.h"

namespace ramiel {

    TextArea::TextArea()
        : value()
        , lines()
        , pos()
        , size()
        , font(nullptr)
        , fontSize(0.0f)
        , fontColor({ 255, 255, 255 })
    {}


    const std::string& TextArea::getValue() const {
        return value;
    }

    size_t TextArea::getLine() const {
        return line;
    }

    size_t TextArea::getNLines() const {
        return lines.size();
    }

    Vec2f TextArea::getPos() const {
        return pos;
    }

    Vec2f TextArea::getSize() const {
        return size;
    }

    const Font* TextArea::getFont() const {
        return font;
    }

    float TextArea::getFontSize() const {
        return fontSize;
    }

    Vec3f TextArea::getFontColor() const {
        return fontColor;
    }


    void TextArea::setValue(const std::string& value) {
        this->value = value;
        calcLines();
    }

    void TextArea::setLine(size_t line) {
        assert(line < lines.size());
        this->line = line;
    }

    void TextArea::setPos(Vec2f pos) {
        assert(pos >= 0.0f);
        this->pos = pos;
    }

    void TextArea::setSize(Vec2f size) {
        assert(size >= 0);
        this->size = size;
        calcLines();
    }

    void TextArea::setFont(const Font* font) {
        assert(font);
        this->font = font;
        calcLines();
    }

    void TextArea::setFontSize(float fontSize) {
        assert(fontSize >= 0.0f);
        this->fontSize = fontSize;
        calcLines();
    }

    void TextArea::setFontColor(Vec3f fontColor) {
        assert(fontColor >= 0.0f);
        this->fontColor = fontColor;
    }


    void TextArea::goToBottom() {
        int nScreen = size[Y] / fontSize;
        int nLine = lines.size();
        line = std::max(0, nLine - nScreen);
    }


    void TextArea::insert(const std::string& str) {
        insert(value.size(), str);
    }

    void TextArea::insert(size_t i, const std::string& str) {
        value.insert(i, str);
        calcLines(i);
    }

    void TextArea::erase(size_t i, size_t n) {
        value.erase(i, n);
        calcLines(i);
    }


    void TextArea::render() const {
        size_t i = line ? lines[line - 1] : 0;
        Vec2f pen = { pos[X], pos[Y] + size[Y] - fontSize };

        for (size_t l = line; l < lines.size(); l++) {
            for (i; i < lines[l]; i++) {
                char c = value[i];
                if (c == '\n') continue;

                auto g = font->find(c);
                if (g == font->end()) continue;

                g->second.draw(pen, fontSize, fontColor);
                
                pen[X] += g->second.getWidth() * fontSize;
            }

            pen[X] = pos[X];
            pen[Y] -= fontSize;

            if (pen[Y] < pos[Y]) return;
        }
    }


    void TextArea::calcLines(size_t vi) {
        if (value.empty() || !size) {
            lines.clear();
            return;
        }

        auto it = std::lower_bound(lines.begin(), lines.end(), vi);
        lines.erase(it, lines.end());
        
        const size_t beg = lines.empty() ? 0 : *(it - 1);
        float x = 0.0f;
        for (size_t i = beg; i < value.size(); i++) {
            char c = value[i];

            if (c == '\n') {
                lines.push_back(i + 1);
                x = 0.0f;
                continue;
            }

            auto glyph = font->find(c);
            if (glyph == font->end()) continue;
            float width = glyph->second.getWidth() * fontSize;

            if (x + width <= size[X]) {
                x += width;
                continue;
            }

            lines.push_back(i);
            x = width;
        }

        if (lines.empty() || lines[lines.size() - 1] != value.size()) {
            lines.push_back(value.size());
        }
    }

}

