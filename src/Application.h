//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#ifndef IMAGE_PROCESSING_APPLICATION_H
#define IMAGE_PROCESSING_APPLICATION_H

#include <memory>
#include <cstdint>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <spdlog/spdlog.h>

class Application {
private:
    std::shared_ptr<Pistache::Address> addr;

    std::shared_ptr<Pistache::Http::Endpoint> server;

    std::shared_ptr<Pistache::Rest::Router> router;

public:

    Application(const std::string &host, int16_t port, int8_t threads);

    ~Application();

    void start();

    void stop();

    template<typename Result, typename Cls, typename... Args, typename Obj>
    void addGetRoute(const std::string &route, Result (Cls::*func)(Args...), std::shared_ptr<Obj> obj) {
        spdlog::info("Adding GET route {}.", route);
        Pistache::Rest::Routes::Get(*router, route, Pistache::Rest::Routes::bind(func, obj));
    }

    template<typename Result, typename Cls, typename... Args, typename Obj>
    void addPostRoute(const std::string &route, Result (Cls::*func)(Args...), std::shared_ptr<Obj> obj) {
        spdlog::info("Adding POST route {}.", route);
        Pistache::Rest::Routes::Post(*router, route, Pistache::Rest::Routes::bind(func, obj));
    }
};


#endif //IMAGE_PROCESSING_APPLICATION_H
