//
// Created by Savu Liviu-Gabriel on 4/24/21.
//

#ifndef IMAGE_PROCESSING_FORMDATA_H
#define IMAGE_PROCESSING_FORMDATA_H

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class FormData {
private:
    std::map<std::string, std::string> attributes;
    std::string content;

public:
    FormData() { }

    void setContent(const std::string &content);

    const std::string &getContent() const;

    const std::string getAttribute(std::string key) const;

    void addAttribute(std::string key, std::string value);

    friend std::ostream &operator<<(std::ostream &out, const FormData &c);

};


#endif //IMAGE_PROCESSING_FORMDATA_H
