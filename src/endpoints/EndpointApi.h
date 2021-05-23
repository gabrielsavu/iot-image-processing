//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#ifndef IMAGE_PROCESSING_ENDPOINTAPI_H
#define IMAGE_PROCESSING_ENDPOINTAPI_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <pistache/http.h>
#include <pistache/router.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <random>

class EndpointApi {

protected:
    /// Transform RGB image into black and white image.
    /// \param RGB image
    /// \return black and white image
    cv::Mat transformToBWImage(const cv::Mat &image);

    /// Remove the noise from the image. (Some white dots are still present after black and white processing).
    /// \param image the black and white image
    /// \return a clean image without noise
    cv::Mat undesiredObjects(const cv::Mat &image);

    /// Transform de image to black and white and calibrate the image in spec to camera intrinsic.
    /// \param image the image to be transformed
    /// \param imagePoints 2d image points that correspond to the object points
    /// \param objectPoints 3d object points that correspond to image points
    /// \param cameraMatrix camera matrix (intrinsic parameter)
    /// \param distCoefficients dist coefficients (intrinsic parameter)
    /// \param rotationVector camera rotation (extrinsic parameter)
    /// \param translationVector camera translation (extrinsic parameter)
    /// \param preprocessing if true then the black and white image will be returned, otherwise the original image will be returned.
    /// \return check preprocessing param
    cv::Mat calibrate(const cv::Mat &image, std::vector<cv::Point2f> imagePoints,
                      std::vector<cv::Point3f> objectPoints, cv::Mat cameraMatrix,
                      cv::Mat distCoefficients, cv::Mat &rotationVector, cv::Mat &translationVector,
                      bool preprocessing);


    /// Transform the buffer into opencv matrix.
    /// The buffer must be an image.
    /// \param content the buffer
    /// \return opencv matrix
    cv::Mat fromStringToImage(std::string content);

    /// Transform the buffer into opencv 2d points.
    /// \param content the buffer
    /// \return vector of 2d points
    std::vector<cv::Point2f> fromStringToPoints2D(std::string content);

    /// Transform the buffer into 3d points.
    /// \param content the buffer
    /// \return vector of 3d points
    std::vector<cv::Point3f> fromStringToPoints3D(std::string content);

    /// Transform the buffer into opencv matrix.
    /// \param content the buffer
    /// \param rows number of rows
    /// \param cols number of columns
    /// \return the matrix
    cv::Mat fromStringToMatrix(std::string content, int16_t rows, int16_t cols);

    /// Project a 3d point to a 2d point.
    /// \param image tuple of <black and white image, camera matrix, dist coefficients, rotation vector, translation vector>
    /// \param point the 3d point
    /// \return the projected point on the image
    cv::Point2f projectPoint(std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat> image, cv::Point3f point);

    /// Stolen from https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
    cv::Point3f randomPoint3D(uint32_t xRangeFrom, uint32_t xRangeTo,
                              uint32_t yRangeFrom, uint32_t yRangeTo,
                              uint32_t zRangeFrom, uint32_t zRangeTo);

    cv::Point2f randomPoint2D(uint32_t xRangeFrom, uint32_t xRangeTo,
                              uint32_t yRangeFrom, uint32_t yRangeTo);


    std::vector<uchar> encodeImage(std::string encoding, cv::Mat image);

public:
    /// This function handles the request for this specific endpoint.
    /// \param request the request object
    /// \param response the response object
    virtual void handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) = 0;

};


#endif //IMAGE_PROCESSING_ENDPOINTAPI_H
