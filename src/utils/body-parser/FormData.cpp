//
// Created by Savu Liviu-Gabriel on 4/24/21.
//

#include "FormData.h"

const std::string FormData::getAttribute(std::string key) const {
    spdlog::info("get(key: {}) - start", key);
    auto value = this->attributes.find(key);
    if (value != this->attributes.end()) {
        spdlog::info("get(key: {}) - end, found: {}", key, value->second);
        return value->second;
    } else {
        spdlog::info("get(key: {}) - end, return empty string", key);
        return std::string();
    }
}

void FormData::addAttribute(std::string key, std::string value) {
    this->attributes[key] = value;
}

const std::string &FormData::getContent() const {
    return content;
}

void FormData::setContent(const std::string &content) {
    FormData::content = content;
}

std::ostream &operator<<(std::ostream &out, const FormData &obj) {
    out << "(";
    for (auto it = obj.attributes.begin(); it != obj.attributes.end(); ++it)
        out << ((it == obj.attributes.begin()) ? "" : " ") << it->first << " => " << it->second;
    out << ")";
    return out;
}
