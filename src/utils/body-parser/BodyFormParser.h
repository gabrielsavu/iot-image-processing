//
// Created by Savu Liviu-Gabriel on 4/24/21.
//

#ifndef IMAGE_PROCESSING_BODYFORMPARSER_H
#define IMAGE_PROCESSING_BODYFORMPARSER_H


#include <memory>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "FormData.h"

/// https://tools.ietf.org/html/rfc2046
class BodyFormParser {

private:

    static const boost::regex content_regex;

    std::shared_ptr<std::string> body;

    std::shared_ptr<std::vector<std::shared_ptr<FormData>>> parsed;

public:
    BodyFormParser(std::string body) {
        this->body = std::make_shared<std::string>(body);
        this->parsed = std::make_shared<std::vector<std::shared_ptr<FormData>>>();
    }

    bool parse();

    std::shared_ptr<FormData> get(std::string name);

};


#endif //IMAGE_PROCESSING_BODYFORMPARSER_H
