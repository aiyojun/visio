//
// Created by aiyo on 11/23/19.
//

#include "CameraProcessor.h"
#include "ImageUtil.h"
#include "YamlUtil.h"
#include "GRuntime.h"
#include <thread>
#include <glog/logging.h>
#include <future>
#include <opencv2/highgui.hpp>
#include <proc/ImageProcessor.h>
#include <opencv2/imgproc.hpp>

CameraProcessor* CameraProcessor::self = nullptr;

CameraProcessor::CameraProcessor(): align(rs2::align(RS2_STREAM_COLOR)) {

}

CameraProcessor::~CameraProcessor() {

}

void CameraProcessor::start() {
    if (running) return;
    running = true;
    if (isFirstTime || !ready) {
        prepare();
        isFirstTime = false;
    }
    ptd = new std::thread(CameraProcessor::loop);
}

void CameraProcessor::loop() {
    auto *pc = GRuntime::ref().pCam;
    if (!pc->ready) {
        GRuntime::ref().pStatusBar->showMessage("No device detected. Is it plugged in?");
        return;
    }
    ImageProcessor imageProcessor;
    LOG(INFO) << "Thread of camera started, enter loop do image acquisition.";
    while (pc->isRunning()) {
        auto frames = pc->pipeline.wait_for_frames();
        if (CameraProcessor::profileChanged(pc->pipeline.get_active_profile().get_streams(), pc->prof.get_streams())) {
            pc->prof = pc->pipeline.get_active_profile();
            pc->intr = pc->prof.get_stream(rs2_stream::RS2_STREAM_DEPTH)
                    .as<rs2::video_stream_profile>().get_intrinsics();
            pc->align = rs2::align(RS2_STREAM_COLOR);
        }
        auto alignedFrames = pc->align.process(frames);
        cv::Mat mask;
        std::vector<PointsGroup> points;
	   LOG(INFO) << "depth proc";
        //cv::Mat depthImage = ImageProcessor::depth0proc(
        //        ImageUtil::convertRealsense2Mat(alignedFrames.get_depth_frame(), CV_8UC1));
	   rs2::depth_frame depthImage = alignedFrames.get_depth_frame();
	//        cv::Mat depthImage = ImageUtil::convertRealsense2Mat(alignedFrames.get_depth_frame(), CV_32FC1);
        /**
         * key point: image processing
         */
	   LOG(INFO) << "image proc";
        cv::Mat processedColorImage;
        bool isDesired = imageProcessor.process(
                processedColorImage,mask, points,
                ImageUtil::convertRealsense2Mat(alignedFrames.get_color_frame()),
                depthImage, &pc->intr
        );
        cv::Mat depthColor, dst;
        std::vector<cv::Mat> color{processedColorImage};
        GRuntime::ref().pImageCache1->cacheBatchImage(color);
        if (pc->sampleMode && isDesired) {
		LOG(INFO) << "sample proc";
            pc->sampleMode = false;
            try {
                GRuntime::ref().pSer->flush();
            } catch (...) {
                /// here, do nothing
            }
            break;
        }
    }
    pc->running = false;
    LOG(INFO) << "Exit camera thread.";
}

CameraProcessor *CameraProcessor::buildAndGet() {
    if (!CameraProcessor::self) {
        LOG(INFO) << "First time to create camera processor.";
        CameraProcessor::self = new CameraProcessor();
        GRuntime::ref().pCam = self;
    }
    return CameraProcessor::self;
}

void CameraProcessor::prepare() {
    auto devicesList = realsenseContex.query_devices();
    if (devicesList.size() == 0) {
        LOG(ERROR) << "No device detected. Is it plugged in?";
        GRuntime::ref().pStatusBar->showMessage("no device detected. Is it plugged in?");
        return;
    }
    ready = true;
    LOG(ERROR) << "Detected device! All [ " << devicesList.size() << " ].";
    realsenseCamera = devicesList.front();

    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    LOG(INFO) << "Starting realsense pipeline ... ...";
    GRuntime::ref().pStatusBar->showMessage("linking realsense camera ... ...");
    prof = pipeline.start(cfg); // start camera with default configuration
    /// get cameta intrinsics
    intr = prof.get_stream(rs2_stream::RS2_STREAM_DEPTH).as<rs2::video_stream_profile>().get_intrinsics();
    LOG(INFO) << "Complete initialize camera.";
}

void CameraProcessor::close() {
    if (!running) {
        GRuntime::ref().pStatusBar->showMessage("link first.");
        return;
    }
    GRuntime::ref().pStatusBar->showMessage("stop display.");
    LOG(INFO) << "Close camera thread.";
    running = false;
}

bool CameraProcessor::isRunning() {
    return running;
}

void CameraProcessor::joinCameraThread() {
    if (ptd != nullptr && ptd->joinable()) {
        ptd->join();
    }
}


bool CameraProcessor::profileChanged(const std::vector<rs2::stream_profile>& current, const std::vector<rs2::stream_profile>& prev) {
    for (auto&& sp : prev) {
        auto itr = std::find_if(std::begin(current), std::end(current), [&sp](const rs2::stream_profile& current_sp) {
            return sp.unique_id() == current_sp.unique_id();
        });
        if (itr == std::end(current)) {
            return true;
        }
    }
    return false;
}
