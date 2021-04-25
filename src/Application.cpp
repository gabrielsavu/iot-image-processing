//
// Created by Savu Liviu-Gabriel on 4/23/21.
//

#include "Application.h"


Application::Application(const std::string &host, int16_t port, int8_t threads) {
    addr = std::make_shared<Pistache::Address>(host, Pistache::Port(port));
    server = std::make_shared<Pistache::Http::Endpoint>(*addr);
    auto opts = Pistache::Http::Endpoint::options();
    opts.threads(threads);
    opts.maxRequestSize(52428800);
    server->init(opts);
    router = std::make_shared<Pistache::Rest::Router>();
    spdlog::info("Threads: {} Host: {} Port: {}", threads, host, port);
}

void Application::start() {
    spdlog::info("Application starting...");
    server->setHandler(router->handler());
    server->serveThreaded();
}

void Application::stop() {
    spdlog::info("Application shutting down...");
    server->shutdown();
}

Application::~Application() {
    server->shutdown();
}