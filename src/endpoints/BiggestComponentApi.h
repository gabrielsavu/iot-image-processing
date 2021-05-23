//
// Created by hyperledger on 5/23/21.
//

#ifndef IMAGE_PROCESSING_BIGGESTCOMPONENTAPI_H
#define IMAGE_PROCESSING_BIGGESTCOMPONENTAPI_H


#include "EndpointApi.h"
#include <utils/body-parser/BodyFormParser.h>

class BiggestComponentApi : public EndpointApi {
public:
    /// This function handles the request for this specific endpoint.
    /// \param request the request object
    /// \param response the response object
    void handle(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
};


#endif //IMAGE_PROCESSING_BIGGESTCOMPONENTAPI_H
