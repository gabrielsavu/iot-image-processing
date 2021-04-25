#include <signal.h>
#include "Application.h"
#include "endpoints/VolumeApi.h"
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_sinks.h>


int main(int argc, char **argv) {

    spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");

    sigset_t signals;
    if (sigemptyset(&signals) != 0
        || sigaddset(&signals, SIGTERM) != 0
        || sigaddset(&signals, SIGINT) != 0
        || sigaddset(&signals, SIGHUP) != 0
        || pthread_sigmask(SIG_BLOCK, &signals, nullptr) != 0) {
        spdlog::error("Install signal handler failed");
        perror("Install signal handler failed");
        return 1;
    }

    auto volumeApi = std::make_shared<VolumeApi>();

    Application application("localhost", 8080, 4);
    application.addPostRoute("/volume", &VolumeApi::handle, volumeApi);
    application.start();


    int signal = 0;
    int status = sigwait(&signals, &signal);
    if (status == 0) {
        spdlog::info("Received signal {}.", signal);
    } else {
        spdlog::info("Sigwait returns {}.", status);
    }

    application.stop();

    return 0;
}

/*
int main(int argc, char **argv) {

    std::string body("----------------------------475828054412968794313598\n"
                     "Content-Disposition: form-data; name=\"imagePoints\"\n"
                     "\n"
                     "750 6950\n"
                     "4972 6412\n"
                     "4912 1572\n"
                     "688 2140\n"
                     "----------------------------475828054412968794313598\n"
                     "Content-Disposition: form-data; name=\"objectPoints\"\n"
                     "\n"
                     "0 0 28\n"
                     "40 0 28\n"
                     "40 40 28\n"
                     "0 40 28\n"
                     "----------------------------475828054412968794313598--");

    BodyFormParser bodyFormParser(body);

    bodyFormParser.parse();

    return 0;
}*/