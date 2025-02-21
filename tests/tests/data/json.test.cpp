#include <ramiel/test.h>
#include <ramiel/util.h>
#include <ramiel/file.h>
#include <ramiel/data.h>
using namespace ramiel;
using namespace ramiel::test;

const std::string dataDir = ramiel_TEST_DATA_DIR;

RAMIEL_TEST_ADD(Json) {
    std::string jsonString = readFile(dataDir + "/data.json");
    JsonValue::H json = jsonParse(jsonString);

    JsonObject::H root = asJsonObject(json);
    RAMIEL_TEST_ASSERT(root);
    if (!root) return;
    JsonObject::H root2 = asJsonObject(root->copy());

    JsonValue::H nothing = json;
    RAMIEL_TEST_ASSERT(root->get("nothing", nothing));
    RAMIEL_TEST_ASSERT(nothing == nullptr);

    JsonBool::H happy = asJsonBool(root->get("happy"));
    RAMIEL_TEST_ASSERT(happy);
    if (happy) RAMIEL_TEST_ASSERT(happy->get());

    JsonNumber::H pi = asJsonNumber(root->get("pi"));
    RAMIEL_TEST_ASSERT(pi);
    if (pi) RAMIEL_TEST_ASSERT(equal(pi->get(), 3.141f));

    JsonString::H name = asJsonString(root->get("name"));
    RAMIEL_TEST_ASSERT(name);
    if (name) RAMIEL_TEST_ASSERT(name->get() == "Niels");

    JsonArray::H list = asJsonArray(root->get("list"));
    RAMIEL_TEST_ASSERT(list);
    if (list) {
        if (list->size() == 3) {
            JsonNumber::H listElem = asJsonNumber(list->get(2));
            RAMIEL_TEST_ASSERT(listElem);
            if (listElem) RAMIEL_TEST_ASSERT(listElem->get() == 2.0f);

            list->erase(1);
            RAMIEL_TEST_ASSERT(list->size() == 2);

            list->insert(makeJsonString("zero"));
            RAMIEL_TEST_ASSERT(list->size() == 3);
        }
    }

    JsonObject::H object = asJsonObject(root->get("object"));
    RAMIEL_TEST_ASSERT(object);
    if (object) {
        JsonString::H currency = asJsonString(object->get("currency"));
        RAMIEL_TEST_ASSERT(currency);
        if (currency) {
            RAMIEL_TEST_ASSERT(currency->get() == "USD");
            currency->set("CAD");
        }
    }

    root2->erase("pi");
    RAMIEL_TEST_ASSERT(root2->size() == 6);

    root2->insert("root2", makeJsonNumber(1.414f));
    RAMIEL_TEST_ASSERT(root2->size() == 7);

    // JsonObject uses std::unordered_map so we cant check for exact match
    std::string stringified = root->stringify();
    RAMIEL_TEST_ASSERT(stringified.size() == 148);
    RAMIEL_TEST_ASSERT(stringified.find("\"list\":[1,2,\"zero\"]") != std::string::npos);
    RAMIEL_TEST_ASSERT(stringified.find("root2") == std::string::npos);
}
