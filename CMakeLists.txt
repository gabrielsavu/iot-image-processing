cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
project(image-processing)
find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)
find_package(OpenCV REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost 1.36.0 COMPONENTS regex algorithm)
include_directories(
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src
)
add_executable(image-processing
        src/main.cpp
        src/endpoints/VolumeApi.cpp
        src/endpoints/VolumeApi.h
        src/Application.cpp
        src/Application.h
        src/endpoints/EndpointApi.cpp
        src/endpoints/EndpointApi.h
        src/utils/body-parser/BodyFormParser.cpp
        src/utils/body-parser/BodyFormParser.h
        src/utils/body-parser/FormData.cpp
        src/utils/body-parser/FormData.h)
target_link_libraries(image-processing
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
        PkgConfig::Pistache
        spdlog::spdlog)
