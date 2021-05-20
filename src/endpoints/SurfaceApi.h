//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#ifndef IMAGE_PROCESSING_SURFACEAPI_H
#define IMAGE_PROCESSING_SURFACEAPI_H

#include <opencv2/opencv.hpp>
#include <pistache/router.h>
#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"
#include "EndpointApi.h"
#include <boost/regex.hpp>
#include <fstream>
#include <utils/body-parser/BodyFormParser.h>
#include <iostream>

class SurfaceApi : public EndpointApi {
private:
    /// Calculates the SURFACE of an object given in the images. It is using Monte-Carlo simulation.
    /// Each image must be black and white and preprocessed by calibrate function.
    /// \param images a vector of tuples <black and white image, camera matrix, dist coefficients, rotation vector, translation vector>
    /// \param numberOfPoints
    /// \return the SURFACE
    float_t calculateSurface(std::vector<std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat>> images,
                            uint32_t numberOfPoints, uint32_t xBoxRangeFrom, uint32_t xBoxRangeTo,
                            uint32_t yBoxRangeFrom, uint32_t yBoxRangeTo);

public:
    /// This function handles the request for this specific endpoint.
    /// \param request the request object
    /// \param response the response object
    void handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
};

#endif //IMAGE_PROCESSING_SURFACEAPI_H
