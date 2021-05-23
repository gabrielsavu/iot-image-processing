//
// Created by hyperledger on 5/23/21.
//

#include "RoiApi.h"

void RoiApi::handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    spdlog::info("handle() - start, body.size: {}", request.body().size());

    BodyFormParser bodyFormParser(request.body());
    bodyFormParser.parse();

    auto image = bodyFormParser.get("image");
    auto xTop = bodyFormParser.get("xTop");
    auto yTop = bodyFormParser.get("yTop");
    auto height = bodyFormParser.get("height");
    auto width = bodyFormParser.get("width");

    if (image == nullptr || xTop == nullptr || yTop == nullptr || height == nullptr || width == nullptr) {
        spdlog::info("handle() - end, Not_Acceptable");
        response.send(Pistache::Http::Code::Not_Acceptable);
        return;
    }

    auto imageDecoded = this->fromStringToImage(image->getContent());
    spdlog::info("handle() - image  height: {} width: {}", imageDecoded.rows, imageDecoded.cols);

    int newWidth = std::stoi(width->getContent().c_str());
    int newHeight = std::stoi(height->getContent().c_str());
    cv::Rect roi(std::stoi(xTop->getContent().c_str()), std::stoi(yTop->getContent().c_str()), newWidth, newHeight);
    cv::Mat newImage = imageDecoded(roi);

    std::vector<uchar> encodedJpeg = this->encodeImage(".jpg", newImage);
    spdlog::info("handle() - image BW size: {}", encodedJpeg.size());

    response.send(Pistache::Http::Code::Ok, std::string(encodedJpeg.begin(), encodedJpeg.end()));
}