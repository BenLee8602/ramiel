#pragma once

#include <string>
#include <sstream>
#include <charconv>
#include <algorithm>

#include <ramiel/math.h>

namespace ramiel {

    template<typename T>
    std::string toString(const T& in) {
        return std::to_string(in);
    }

    template<typename T, size_t N>
    std::string toString(const Vec<T, N>& in) {
        std::ostringstream out;
        for (size_t i = 0; i < N - 1; i++) {
            out << toString(in[i]) << ',';
        }
        out << toString(in[N - 1]);
        return out.str();
    }

    template<typename T>
    bool fromString(const std::string& in, T& out) {
        auto first = in.data();
        auto last = in.data() + in.size();
        auto res = std::from_chars(first, last, out);
        return res.ec == std::errc() && res.ptr == last;
    }

    template<typename T, size_t N>
    bool fromString(const std::string& in, Vec<T, N>& out) {
        if (std::count(in.begin(), in.end(), ',') != N - 1) return false;
        std::istringstream str(in);
        for (size_t i = 0; i < N; i++) {
            std::string elem;
            if (!std::getline(str, elem, ',')) return false;
            if (!fromString(elem, out[i])) return false;
        }
        return true;
    }

}
