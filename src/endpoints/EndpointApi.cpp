//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#include "EndpointApi.h"

cv::Mat EndpointApi::calibrate(const cv::Mat &image, std::vector<cv::Point2f> imagePoints,
                               std::vector<cv::Point3f> objectPoints, cv::Mat cameraMatrix,
                               cv::Mat distCoefficients, cv::Mat &rotationVector, cv::Mat &translationVector,
                               bool preprocessing) {
    spdlog::info("loadAndCalibrate() - start");
    cv::Mat tmpImage;
    if (preprocessing) {
        tmpImage = transformToBWImage(image);
    } else {
        tmpImage = cv::Mat(image);
    }

    cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoefficients, rotationVector, translationVector);
    spdlog::info("loadAndCalibrate() - end");
    return tmpImage;
}


cv::Mat EndpointApi::transformToBWImage(const cv::Mat &image) {
    spdlog::info("transformToBWImage() - start");
    cv::Mat greyImage;
    cv::cvtColor(image, greyImage, cv::COLOR_RGB2GRAY);
    cv::Mat imageBlurred;
    cv::GaussianBlur(greyImage, imageBlurred, cv::Size(3, 3), 0);
    cv::Mat imageThreshold;
    cv::threshold(imageBlurred, imageThreshold, 127, 255, cv::THRESH_TRIANGLE);
    spdlog::info("transformToBWImage() - end");
    return undesiredObjects(imageThreshold);
}

cv::Mat EndpointApi::undesiredObjects(const cv::Mat &image) {
    spdlog::info("undesiredObjects() - start");
    cv::Mat labels, stats, centroids;
    int nb_components = cv::connectedComponentsWithStats(image, labels, stats, centroids, 8);

    std::vector<int> sizes(stats.rows);
    int col = stats.cols - 1;
    for (int row = 0; row < stats.rows; row++) {
        sizes[row] = stats.at<int>(row, col);
    }
    spdlog::info("undesiredObjects() - sizes: {}", sizes.size());

    int maxLabels = 1;
    int maxSize = sizes[1];
    for (int i = 2; i < nb_components; i++) {
        if (sizes[i] > maxSize) {
            maxLabels = i;
            maxSize = sizes[i];
        }
    }
    spdlog::info("undesiredObjects() - maxLabels: {} maxSize: {} imageType: {}", maxLabels, maxSize, image.type());
    for (int row = 0; row < labels.rows; row++) {
        for (col = 0; col < labels.cols; col++) {
            if (labels.at<int>(row, col) == maxLabels) {
                labels.at<int>(row, col) = 255;
            } else {
                labels.at<int>(row, col) = 0;
            }
        }
    }
    cv::Mat dstImg;
    labels.convertTo(dstImg, CV_8U);
    spdlog::info("undesiredObjects() - end");
    return dstImg;
}

cv::Mat EndpointApi::fromStringToImage(std::string content) {
    std::vector<unsigned char> vectorImage(content.begin(), content.end());
    cv::Mat imageMat(vectorImage, true);
    cv::Mat imageDecoded(cv::imdecode(imageMat, 1));
    return imageDecoded;
}


std::ostream &operator<<(std::ostream &os, const cv::Point3f &point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
}

std::istream &operator>>(std::istream &is, cv::Point3f &point) {
    is >> point.x >> point.y >> point.z;
    return is;
}

std::ostream &operator<<(std::ostream &os, const cv::Point2f &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

std::istream &operator>>(std::istream &is, cv::Point2f &point) {
    is >> point.x >> point.y;
    return is;
}

std::vector<cv::Point2f> EndpointApi::fromStringToPoints2D(std::string content) {
    std::vector<cv::Point2f> vector;
    std::istringstream iss(content);
    cv::Point2f temp;
    while (iss >> temp) {
        vector.push_back(temp);
    }
    return vector;
}

std::vector<cv::Point3f> EndpointApi::fromStringToPoints3D(std::string content) {
    std::vector<cv::Point3f> vector;
    std::istringstream iss(content);
    cv::Point3f temp;
    while (iss >> temp) {
        vector.push_back(temp);
    }
    return vector;
}

cv::Mat EndpointApi::fromStringToMatrix(std::string content, int16_t rows, int16_t cols) {
    cv::Mat mat(rows, cols, CV_32F);
    std::istringstream iss(content);
    float_t temp;
    for (int16_t i = 0; i < rows; i++) {
        for (int16_t j = 0; j < rows; j++) {
            iss >> temp;
            mat.at<float_t>(i, j) = temp;
        }
    }
    return mat;
}

cv::Point2f EndpointApi::projectPoint(std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat, cv::Mat> image,
                                      cv::Point3f point) {
    // Order image tuple: calibratedImage, cameraMatrixMat, distCoefficientsMat, rotationVector, translationVector
    std::vector<cv::Point3f> objectPoints;
    std::vector<cv::Point2f> imagePoints;

    objectPoints.push_back(point);

    cv::projectPoints(objectPoints, std::get<3>(image), std::get<4>(image),
                      std::get<1>(image), std::get<2>(image), imagePoints);

    return imagePoints[0];
}

cv::Point3f EndpointApi::randomPoint3D(uint32_t xRangeFrom, uint32_t xRangeTo,
                                       uint32_t yRangeFrom, uint32_t yRangeTo,
                                       uint32_t zRangeFrom, uint32_t zRangeTo) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<uint32_t> distributionX(xRangeFrom, xRangeTo);
    std::uniform_int_distribution<uint32_t> distributionY(yRangeFrom, yRangeTo);
    std::uniform_int_distribution<uint32_t> distributionZ(zRangeFrom, zRangeTo);
    cv::Point3f point;
    point.x = distributionX(generator);
    point.y = distributionY(generator);
    point.z = distributionZ(generator);
    return point;
}

cv::Point2f EndpointApi::randomPoint2D(uint32_t xRangeFrom, uint32_t xRangeTo,
                                       uint32_t yRangeFrom, uint32_t yRangeTo) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<uint32_t> distributionX(xRangeFrom, xRangeTo);
    std::uniform_int_distribution<uint32_t> distributionY(yRangeFrom, yRangeTo);

    cv::Point2f point;
    point.x = distributionX(generator);
    point.y = distributionY(generator);

    return point;
}

std::vector<uchar> EndpointApi::encodeImage(std::string encoding, cv::Mat image) {
    std::vector<uchar> buffer;
    cv::imencode(encoding, image, buffer);
    return buffer;
}