//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#include "SurfaceApi.h"

void SurfaceApi::handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
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
    float surface = calculateSurface(imageBW, 20000, 10, 30, 10, 30);

    spdlog::info("handle() - surface: {}", surface);

    response.send(Pistache::Http::Code::Ok, std::to_string(surface));
}


//https://mathonweb.com/entrtain/monte/t_monte.htm
float_t SurfaceApi::calculateSurface(cv::Mat image,
                                     uint32_t numberOfPoints, uint32_t xBoxRangeFrom, uint32_t xBoxRangeTo,
                                     uint32_t yBoxRangeFrom, uint32_t yBoxRangeTo) {

    std::vector<cv::Point2f> points(numberOfPoints);

    for (int j = 0; j < numberOfPoints; j++) {
        auto tempPoints = this->randomPoint2D(xBoxRangeFrom, xBoxRangeTo, yBoxRangeFrom, yBoxRangeTo);
        points[j] = tempPoints;
    }

    float_t rectangleArea = (xBoxRangeTo - xBoxRangeFrom) / (yBoxRangeTo - yBoxRangeFrom);

    uint32_t pointsInObject = 0;
    // walk to all points
    for (uint32_t k = 0; k < numberOfPoints; k++) {
        // white dot, in object
        if (image.at<int>(points[k].x, points[k].y) != 0) {
            pointsInObject++;
        }
    }
    float_t surface = rectangleArea * (pointsInObject / numberOfPoints);
    return surface;
}
