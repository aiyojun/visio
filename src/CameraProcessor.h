//
// Created by aiyo on 11/23/19.
//

#ifndef VISIO_CAMERAPROCESSOR_H
#define VISIO_CAMERAPROCESSOR_H


#include "ImageCache.h"
#include <librealsense2/rs.hpp>
#include <thread>
#include <gui/ImageContainer.h>
#include <gui/BottomStatus.h>

class CameraProcessor {
public:
    CameraProcessor();
    ~CameraProcessor();
    void prepare();
    void start();
    void joinCameraThread();
    static CameraProcessor *buildAndGet();
    void close();
    bool isRunning();
    static bool profileChanged(const std::vector<rs2::stream_profile>& current, const std::vector<rs2::stream_profile>& prev);
    bool sampleMode = false;
private:
    static void loop();
    static CameraProcessor *self;

    rs2::context realsenseContex;
    rs2::device realsenseCamera;
    rs2::pipeline pipeline;

    rs2::config cfg;
    rs2::pipeline_profile prof;
    rs2::align align;
    rs2_intrinsics intr;
    float scale = 1;

    bool isFirstTime = true;
    bool ready = false;

    std::thread *ptd = nullptr;
    bool running = false;
};


#endif //VISIO_CAMERAPROCESSOR_H
