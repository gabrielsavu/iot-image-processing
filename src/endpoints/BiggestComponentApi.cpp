//
// Created by hyperledger on 5/23/21.
//

#include "BiggestComponentApi.h"


void BiggestComponentApi::handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    spdlog::info("handle() - start, body.size: {}", request.body().size());

    BodyFormParser bodyFormParser(request.body());
    bodyFormParser.parse();

    auto image = bodyFormParser.get("image");

    if (image == nullptr) {
        spdlog::info("handle() - end, Not_Acceptable");
        response.send(Pistache::Http::Code::Not_Acceptable);
        return;
    }

    auto imageDecoded = this->fromStringToImage(image->getContent());
    spdlog::info("handle() - image  height: {} width: {}", imageDecoded.rows, imageDecoded.cols);
    auto imageBW = this->transformToBWImage(imageDecoded);

    std::vector<uchar> encodedJpeg = this->encodeImage(".jpg", imageBW);

    spdlog::info("handle() - image BW size: {}", encodedJpeg.size());

    response.send(Pistache::Http::Code::Ok, std::string(encodedJpeg.begin(), encodedJpeg.end()));
}