#include <sstream>
#include <cstring>
#include <cassert>

#include "json.h"
using namespace ramiel;

namespace {

    bool expectWord(std::istringstream& in, const char* word);
    void nextChar(std::istringstream& in);

    bool loadString(std::istringstream& in, std::string& out);
    bool loadValue(std::istringstream& in, JsonValue::H& out);
    bool loadArray(std::istringstream& in, JsonValue::H& out);
    bool loadObject(std::istringstream& in, JsonValue::H& out);


    bool expectWord(std::istringstream& in, const char* word) {
        assert(strlen(word) < 8);
        char val[8] = {};
        in.read(val, strlen(word));
        return !strcmp(val, word);
    }

    void nextChar(std::istringstream& in) {
        while (std::isspace(in.peek())) in.get();
    }


    bool loadString(std::istringstream& in, std::string& out) {
        nextChar(in);
        char c = 0;
        if (!in.get(c) || c != '\"') return false;
        
        while (true) {
            if (!in.get(c)) return false;
            if (c == '\"') return true;
            if (c != '\\') {
                out.push_back(c);
                continue;
            }
            
            // handle escape chars
            if (!in.get(c)) return false;
            else if (c == '\"') out.push_back('\"');
            else if (c == '\\') out.push_back('\\');
            else if (c == '/')  out.push_back('/');
            else if (c == 'b')  out.push_back('\b');
            else if (c == 'f')  out.push_back('\f');
            else if (c == 'n')  out.push_back('\n');
            else if (c == 'r')  out.push_back('\r');
            else if (c == 't')  out.push_back('\t');
            else if (c == 'u') {
                if (!in.get(c)) return false;
                if (!in.get(c)) return false;
                if (!in.get(c)) return false;
                if (!in.get(c)) return false;
                out.push_back('?');
            }
            else return false;
        }

        return false;
    }


    bool loadValue(std::istringstream& in, JsonValue::H& out) {
        out = nullptr;
        nextChar(in);
        char c = in.peek();

        if (c == 'n') return expectWord(in, "null");
        if (c == 'f') {
            if (!expectWord(in, "false")) return false;
            out = std::make_shared<JsonBool>(false);
            return true;
        }
        if (c == 't') {
            if (!expectWord(in, "true")) return false;
            out = std::make_shared<JsonBool>(true);
            return true;
        }
        if (std::isdigit(c)) {
            float num;
            in >> num;
            out = std::make_shared<JsonNumber>(num);
            return true;
        }
        if (c == '"') {
            std::string str;
            if (!loadString(in, str)) return false;
            out = std::make_shared<JsonString>(str);
            return true;
        }
        if (c == '[') return loadArray(in, out);
        if (c == '{') return loadObject(in, out);
        
        return false;
    }


    bool loadArray(std::istringstream& in, JsonValue::H& out) {
        char c = 0;
        out = nullptr;

        nextChar(in);
        if (!in.get(c) || c != '[') return false;
        auto arr = std::make_shared<JsonArray>();
        out = arr;
        
        nextChar(in);
        if (in.peek() == ']') {
            in.get();
            return true;
        }

        while (true) {
            nextChar(in);
            JsonValue::H val = nullptr;
            if (!loadValue(in, val)) break;
            arr->insert(val);

            nextChar(in);
            if (!in.get(c)) break;
            if (c == ',') continue;
            if (c != ']') break;

            return true;
        }

        out = nullptr;
        return false;
    }


    bool loadObject(std::istringstream& in, JsonValue::H& out) {
        char c = 0;
        out = nullptr;

        nextChar(in);
        if (!in.get(c) || c != '{') return false;
        auto arr = std::make_shared<JsonObject>();
        out = arr;
        
        nextChar(in);
        if (in.peek() == '}') {
            in.get();
            return true;
        }

        while (true) {
            nextChar(in);
            std::string key;
            if (!loadString(in, key)) break;

            nextChar(in);
            if (!in.get(c) || c != ':') break;

            nextChar(in);
            JsonValue::H val = nullptr;
            if (!loadValue(in, val)) break;
            
            arr->insert(key, val);

            nextChar(in);
            if (!in.get(c)) break;
            if (c == ',') continue;
            if (c != '}') break;

            return true;
        }

        out = nullptr;
        return false;
    }

}

namespace ramiel {

    JsonBool::JsonBool(bool val)
        : val(val)
    {}


    bool JsonBool::get() const {
        return val;
    }


    void JsonBool::set(bool val) {
        this->val = val;
    }


    JsonValue::H JsonBool::copy() const {
        return std::make_shared<JsonBool>(val);
    }


    std::string JsonBool::stringify() const {
        return val ? "true" : "false";
    }


    JsonNumber::JsonNumber(float val)
        : val(val)
    {}


    float JsonNumber::get() const {
        return val;
    }


    void JsonNumber::set(float val) {
        this->val = val;
    }


    JsonValue::H JsonNumber::copy() const {
        return std::make_shared<JsonNumber>(val);
    }


    std::string JsonNumber::stringify() const {
        std::string out = std::to_string(val);
        out.erase(out.find_last_not_of('0') + 1, std::string::npos);
        if (out.back() == '.') out.pop_back();
        return out;
    }


    JsonString::JsonString(const std::string& val)
        : val(val)
    {}


    const std::string& JsonString::get() const {
        return val;
    }


    void JsonString::set(const std::string& val) {
        this->val = val;
    }


    JsonValue::H JsonString::copy() const {
        return std::make_shared<JsonString>(val);
    }


    std::string JsonString::stringify() const {
        return std::string("\"") + val + '"';
    }


    JsonArray::JsonArray(std::initializer_list<JsonValue::H> val) {
        for (auto& v : val) {
            this->val.push_back(v);
        }
    }


    size_t JsonArray::size() const {
        return val.size();
    }


    JsonValue::H JsonArray::get(size_t i) const {
        return val[i];
    }


    void JsonArray::set(size_t i, JsonValue::H val) {
        this->val[i] = std::shared_ptr<JsonValue>(val);
    }


    void JsonArray::insert(JsonValue::H val) {
        this->val.emplace_back(val);
    }


    void JsonArray::erase(size_t i) {
        val.erase(val.begin() + i);
    }


    JsonValue::H JsonArray::copy() const {
        auto c = std::make_shared<JsonArray>();
        c->val.reserve(val.size());
        for (auto& v : val) {
            c->insert(v ? v->copy() : nullptr);
        }
        return c;
    }


    std::string JsonArray::stringify() const {
        if (val.empty()) return "[]";
        std::string out = "[";
        
        for (auto& v : val) {
            out += v ? v->stringify() + ',' : "null,";
        }

        out.pop_back();
        return out + ']';
    }


    JsonObject::JsonObject(
        std::initializer_list<std::pair<std::string, JsonValue::H>> val
    ) {
        for (auto& v : val) {
            this->val.insert(v);
        }
    }


    size_t JsonObject::size() const {
        return val.size();
    }


    bool JsonObject::has(const std::string& key) const {
        return val.find(key) != val.end();
    }


    JsonValue::H JsonObject::get(const std::string& key) const {
        auto v = val.find(key);
        return v == val.end() ? nullptr : v->second;
    }


    bool JsonObject::get(const std::string& key, JsonValue::H& out) const {
        auto v = val.find(key);
        if (v == val.end()) return false;
        out = v->second;
        return true;
    }


    void JsonObject::insert(const std::string& key, JsonValue::H val) {
        this->val.emplace(key, val);
    }


    void JsonObject::erase(const std::string& key) {
        val.erase(key);
    }


    JsonValue::H JsonObject::copy() const {
        auto c = std::make_shared<JsonObject>();
        c->val.reserve(val.size());
        for (auto& [k, v] : val) {
            c->insert(k, v ? v->copy() : nullptr);
        }
        return c;
    }


    std::string JsonObject::stringify() const{
        if (val.empty()) return "{}";
        std::string out = "{";

        for (auto& [k, v] : val) {
            out += '"';
            out += k;
            out += "\":";

            out += v ? v->stringify() : "null";
            out += ',';
        }

        out.pop_back();
        return out + '}';
    }


    JsonBool::H asJsonBool(JsonValue::H json) {
        return std::dynamic_pointer_cast<JsonBool>(json);
    }

    JsonNumber::H asJsonNumber(JsonValue::H json) {
        return std::dynamic_pointer_cast<JsonNumber>(json);
    }

    JsonString::H asJsonString(JsonValue::H json) {
        return std::dynamic_pointer_cast<JsonString>(json);
    }

    JsonArray::H asJsonArray(JsonValue::H json) {
        return std::dynamic_pointer_cast<JsonArray>(json);
    }

    JsonObject::H asJsonObject(JsonValue::H json) {
        return std::dynamic_pointer_cast<JsonObject>(json);
    }
    
    
    JsonBool::H makeJsonBool(bool val) {
        return std::make_shared<JsonBool>(val);
    }

    JsonNumber::H makeJsonNumber(float val) {
        return std::make_shared<JsonNumber>(val);
    }

    JsonString::H makeJsonString(const std::string& val) {
        return std::make_shared<JsonString>(val);
    }

    JsonArray::H makeJsonArray(std::initializer_list<JsonValue::H> val) {
        return std::make_shared<JsonArray>(val);
    }

    JsonObject::H makeJsonObject(std::initializer_list<std::pair<std::string, JsonValue::H>> val) {
        return std::make_shared<JsonObject>(val);
    }


    JsonValue::H jsonParse(const std::string& json_) {
        std::istringstream json(std::move(json_));
        JsonValue::H data = nullptr;
        return loadValue(json, data) ? data : nullptr;
    }


    std::string jsonStringify(JsonValue::H json) {
        return json ? json->stringify() : "null";
    }

}
