//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#include <stateful/Stateful.h>
#include "VolumeApi.h"

void VolumeApi::handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    spdlog::info("handle() - start, body.size: {}", request.body().size());

    BodyFormParser bodyFormParser(request.body());
    bodyFormParser.parse();

    Stateful stateful = Stateful::Get();
    bool isCameraMatrix = stateful.isState("cameraMatrix");
    bool isDistCoefficients = stateful.isState("distCoefficients");


    auto n = bodyFormParser.get("n");
    auto cameraMatrix = bodyFormParser.get("cameraMatrix");
    auto distCoefficients = bodyFormParser.get("distCoefficients");


    if (n == nullptr) {
        spdlog::info("handle() - end, n not found in body Not_Acceptable");
        response.send(Pistache::Http::Code::Not_Acceptable, "The parameter 'n' not found in the body.");
        return;
    }
    if (!isCameraMatrix && cameraMatrix == nullptr) {
        spdlog::info("handle() - end, cameraMatrix not found in body Not_Acceptable");
        response.send(Pistache::Http::Code::Not_Acceptable, "The camera matrix is required for the first request.");
        return;
    }
    if (!isDistCoefficients && distCoefficients == nullptr) {
        spdlog::info("handle() - end, distCoefficients not found in body Not_Acceptable");
        response.send(Pistache::Http::Code::Not_Acceptable, "The dist coefficients is required for the first request.");
        return;
    }

    if (!isDistCoefficients) {
        stateful.addState("distCoefficients", distCoefficients->getContent());
    }
    if (!isCameraMatrix) {
        stateful.addState("cameraMatrix", cameraMatrix->getContent());
    }
    auto distCoefficientsMat = this->fromStringToMatrix(stateful.getState("distCoefficients"), 5, 1);
    auto cameraMatrixMat = this->fromStringToMatrix(stateful.getState("cameraMatrix"), 3, 3);
    std::vector<std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat>> imagesToProcess;

    for (int i = 0; i < std::atoi(n->getContent().c_str()); i++) {
        auto image = bodyFormParser.get("image" + std::to_string(i));
        auto imagePoints = bodyFormParser.get("imagePoints" + std::to_string(i));
        auto objectPoints = bodyFormParser.get("objectPoints" + std::to_string(i));
        if (image == nullptr || imagePoints == nullptr || objectPoints == nullptr) {
            spdlog::info("handle() - end, Not_Acceptable");
            response.send(Pistache::Http::Code::Not_Acceptable);
            return;
        }

        auto imageDecoded = this->fromStringToImage(image->getContent());
        auto imagePoints2D = this->fromStringToPoints2D(imagePoints->getContent());
        auto objectPoints3D = this->fromStringToPoints3D(objectPoints->getContent());
        cv::Mat rotationVector, translationVector;

        spdlog::info("handle() - image {} height: {} width: {}", i, imageDecoded.rows, imageDecoded.cols);

        auto calibratedImage = this->calibrate(imageDecoded, imagePoints2D, objectPoints3D,
                                               cameraMatrixMat, distCoefficientsMat,
                                               rotationVector, translationVector, true);

        imagesToProcess.push_back(std::make_tuple(calibratedImage, cameraMatrixMat, distCoefficientsMat, rotationVector,
                                                  translationVector));
    }

    float volume = calculateVolume(imagesToProcess, 20000, 10, 30, 10, 30, 0, 28);

    spdlog::info("handle() - volume: {}", volume);

    response.send(Pistache::Http::Code::Ok, std::to_string(volume));
}

float_t VolumeApi::calculateVolume(std::vector<std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat>> images,
                                   uint32_t numberOfPoints, uint32_t xBoxRangeFrom, uint32_t xBoxRangeTo,
                                   uint32_t yBoxRangeFrom, uint32_t yBoxRangeTo,
                                   uint32_t zBoxRangeFrom, uint32_t zBoxRangeTo) {

    std::vector<std::vector<cv::Point3f>> points(images.size(), std::vector<cv::Point3f>(numberOfPoints));
    for (int i = 0; i < images.size(); i++) {
        for (int j = 0; j < numberOfPoints; j++) {
            auto tempPoints = this->randomPoint3D(xBoxRangeFrom, xBoxRangeTo, yBoxRangeFrom, yBoxRangeTo, zBoxRangeFrom,
                                                  zBoxRangeTo);
            points[i][j] = tempPoints;
        }
    }

    float_t boxVolume = (xBoxRangeTo - xBoxRangeFrom) * (yBoxRangeTo - yBoxRangeFrom) * (zBoxRangeTo - zBoxRangeFrom);

    uint32_t sigma = 0;
    // walk to all points
    for (uint32_t k = 0; k < numberOfPoints; k++) {
        int8_t isValidPoint = 1;
        // walk to all images and check if the point is white in all of them
        for (int j = 0; j < images.size(); j++) {

            cv::Point2f projectedPoint = this->projectPoint(images[j], points[j][k]);

            cv::Mat image = std::get<0>(images[j]);
            if (image.at<int>(projectedPoint.x, projectedPoint.y) != 0) {
                isValidPoint *= 0;
            }
        }
        sigma += isValidPoint;
    }
    float_t volume = (boxVolume * sigma / 100) / numberOfPoints;
    return volume;
}
