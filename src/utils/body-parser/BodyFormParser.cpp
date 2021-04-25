//
// Created by Savu Liviu-Gabriel on 4/24/21.
//

#include "BodyFormParser.h"

bool BodyFormParser::parse() {
    spdlog::info("parse() - start");
    parsed->clear();
    std::string content = std::string(*this->body);
    boost::cmatch numberResult;
    std::vector<std::string> portionResult;
    std::vector<std::string> headerContentResult;
    std::vector<std::string> attributesResult;
    std::vector<std::string> keyValueResult;

    bool numberFound = boost::regex_search(content.c_str(), numberResult, boost::regex("([0-9]+)"));
    if (!numberFound) {
        spdlog::info("parse() - could not found number, invalid body.");
        return false;
    }
    std::string uniqueNumber(numberResult[1].first, numberResult[1].second);


    boost::algorithm::split_regex(portionResult, content, boost::regex(
            "[-]+" + uniqueNumber + "[-]*(?:\\r\\n|\\r|\\n)*"));
    if (portionResult.empty()) {
        spdlog::info("parse() - invalid body separator format.");
        return false;
    }
    for (int i = 0; i < portionResult.size(); i++) {
        if (!portionResult[i].empty()) {
            boost::algorithm::split_regex(headerContentResult, portionResult[i], boost::regex(
                    "(?:\r\n){2}"));

            if (headerContentResult.empty()) {
                spdlog::info("parse() - invalid body separator format.");
                return false;
            }

            boost::algorithm::split_regex(attributesResult, headerContentResult[0], boost::regex(
                    ";\\s|;|(?:\\r\\n|\\r|\\n)"));


            if (attributesResult.empty()) {
                spdlog::info("parse() - invalid body attributes format.");
                return false;
            }

            std::shared_ptr<FormData> formData = std::make_shared<FormData>();

            for (std::string keyValue : attributesResult) {

                boost::algorithm::split_regex(keyValueResult, keyValue, boost::regex(
                        ":\\s?|="));

                if (keyValueResult.size() != 2) {
                    spdlog::info("parse() - invalid body attributes format.");
                    return false;
                }

                if (keyValueResult[1][0] == '"') {
                    keyValueResult[1] = keyValueResult[1].substr(1, keyValueResult[1].size() - 2);
                }
                formData->addAttribute(keyValueResult[0], keyValueResult[1]);
            }

            std::stringstream concat;
            for (int j = 1; j < headerContentResult.size(); j++) {
                concat << headerContentResult[j];
            }
            formData->setContent(concat.str());
            spdlog::info("parse() - end, form data: {}", *formData);
            parsed->push_back(formData);
        }
    }
    spdlog::info("parse() - end, parsed {} data", parsed->size());
    return true;
}

std::shared_ptr<FormData> BodyFormParser::get(std::string name) {
    spdlog::info("get(name: {}) - start", name);
    for (std::shared_ptr<FormData> formData : *parsed) {
        auto attribute = formData->getAttribute("name");
        spdlog::info("get(name: {}) - formData: {} attribute: {}", name, *formData, attribute);
        if (!attribute.empty()) {
            if (attribute == name) {
                spdlog::info("get(name: {}) - end, found", name);
                return formData;
            }
        }
    }
    spdlog::info("get(name: {}) - end, not found", name);
    return nullptr;
}

