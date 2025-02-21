#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace ramiel {

    class JsonValue {
    public:
        using H = std::shared_ptr<JsonValue>;
        virtual JsonValue::H copy() const = 0;
        virtual std::string stringify() const = 0;
    };


    class JsonBool : public JsonValue {
    public:
        using H = std::shared_ptr<JsonBool>;

        JsonBool(bool val);

        bool get() const;
        void set(bool val);

        virtual JsonValue::H copy() const override;
        virtual std::string stringify() const override;

    private:
        bool val;
    };


    class JsonNumber : public JsonValue {
    public:
        using H = std::shared_ptr<JsonNumber>;

        JsonNumber(float val);

        float get() const;
        void set(float val);

        virtual JsonValue::H copy() const override;
        virtual std::string stringify() const override;

    private:
        float val;
    };


    class JsonString : public JsonValue {
    public:
        using H = std::shared_ptr<JsonString>;

        JsonString(const std::string& val);

        const std::string& get() const;
        void set(const std::string& val);

        virtual JsonValue::H copy() const override;
        virtual std::string stringify() const override;

    private:
        std::string val;
    };


    class JsonArray : public JsonValue {
    public:
        using H = std::shared_ptr<JsonArray>;

        JsonArray() = default;
        JsonArray(std::initializer_list<JsonValue::H> val);

        size_t size() const;
        JsonValue::H get(size_t i) const;
        void set(size_t i, JsonValue::H val);

        void insert(JsonValue::H val);
        void erase(size_t i);
        
        virtual JsonValue::H copy() const override;
        virtual std::string stringify() const override;

    private:
        std::vector<std::shared_ptr<JsonValue>> val;
    };


    class JsonObject : public JsonValue {
    public:
        using H = std::shared_ptr<JsonObject>;

        JsonObject() = default;
        JsonObject(
            std::initializer_list<std::pair<std::string, JsonValue::H>> val
        );

        size_t size() const;
        bool has(const std::string& key) const;
        bool get(const std::string& key, JsonValue::H& val) const;
        JsonValue::H get(const std::string& key) const;
        
        void insert(const std::string& key, JsonValue::H val);
        void erase(const std::string& key);

        virtual JsonValue::H copy() const override;
        virtual std::string stringify() const override;

    private:
        std::unordered_map<std::string, std::shared_ptr<JsonValue>> val;
    };
    

    JsonBool::H asJsonBool(JsonValue::H json);
    JsonNumber::H asJsonNumber(JsonValue::H json);
    JsonString::H asJsonString(JsonValue::H json);
    JsonArray::H asJsonArray(JsonValue::H json);
    JsonObject::H asJsonObject(JsonValue::H json);

    JsonBool::H makeJsonBool(bool val = false);
    JsonNumber::H makeJsonNumber(float val = 0.0f);
    JsonString::H makeJsonString(const std::string& val = "");
    JsonArray::H makeJsonArray(std::initializer_list<JsonValue::H> val = {});
    JsonObject::H makeJsonObject(std::initializer_list<std::pair<std::string, JsonValue::H>> val = {});


    JsonValue::H jsonParse(const std::string& json);
    std::string jsonStringify(JsonValue::H json);

}
