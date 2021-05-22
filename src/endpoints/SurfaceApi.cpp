//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#include "SurfaceApi.h"

void SurfaceApi::handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    spdlog::info("handle() - start, body.size: {}", request.body().size());

    BodyFormParser bodyFormParser(request.body());
    bodyFormParser.parse();

    auto n = bodyFormParser.get("n");
    auto cameraMatrix = bodyFormParser.get("cameraMatrix");
    
    if (n == nullptr || cameraMatrix == nullptr) {
        spdlog::info("handle() - end, n not found in body Not_Acceptable");
        response.send(Pistache::Http::Code::Not_Acceptable);
        return;
    }

    // auto cameraMatrixMat = this->fromStringToMatrix(cameraMatrix->getContent(), 3, 3);
    
   

    
        auto image = bodyFormParser.get("image");
        auto imagePoints = bodyFormParser.get("imagePoints");

        if (image == nullptr || imagePoints == nullptr) {
            spdlog::info("handle() - end, Not_Acceptable");
            response.send(Pistache::Http::Code::Not_Acceptable);
            return;
        }

        auto imageDecoded = this->fromStringToImage(image->getContent());
        auto imagePoints2D = this->fromStringToPoints2D(imagePoints->getContent());
        std::cout<<"YOOo";


        spdlog::info("handle() - image  height: {} width: {}",  imageDecoded.rows, imageDecoded.cols);

     


    

    float surface = calculateSurface(imageDecoded, 20000, 10, 30, 10, 30, 0, 28);

    spdlog::info("handle() - surface: {}", surface);

    response.send(Pistache::Http::Code::Ok, std::to_string(surface));
}

float_t SurfaceApi::calculateSurface(cv::Mat image,
                                   uint32_t numberOfPoints, uint32_t xBoxRangeFrom, uint32_t xBoxRangeTo,
                                   uint32_t yBoxRangeFrom, uint32_t yBoxRangeTo
                                   ) {

    std::vector<cv::Point2f> points(std::vector<cv::Point2f>(numberOfPoints));

        for (int j = 0; j < numberOfPoints; j++) {
            auto tempPoints = this->randomPoint2D(xBoxRangeFrom, xBoxRangeTo, yBoxRangeFrom, yBoxRangeTo);
            points[j] = tempPoints;
        }
    

    float_t easternPoint=9999;
    float_t southernPoint=99999;
    float_t northernPoint=-99999;
    float_t westernPoint=-999999;
    uint32_t sigma = 0;
    // walk to all points
    for (uint32_t k = 0; k < numberOfPoints; k++) {
        int8_t isValidPoint = 1;
       
            
            if (image.at<int>(points[k].x, points[k].y) != 0) {
                isValidPoint *= 0;
            }
        
        if(isValidPoint==1){
            if(points[j][k].y>northernPoint)
                northernPoint=points[j][k].y;
            else if(points[j][k].y<southernPoint)
                southernPoint=points[j][k].y;
            else  if(points[j][k].x>easternPoint)
                easternPoint=points[j][k].x;
            else if(points[j][k].x<westernPoint)
                westernPoint=points[j][k].x;
            
        }
    }
    float_t surface= (westernPoint-easternPoint) / (northernPoint-southernPoint);
    return surface;
}
