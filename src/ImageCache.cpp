//
// Created by aiyo on 11/23/19.
//

#include "ImageCache.h"
#include <glog/logging.h>

#include <utility>
#include "comm/macro.h"


ImageCache::ImageCache() {
    LOG(INFO) << "image cache constructor";
}

ImageCache::~ImageCache() {

}


bool ImageCache::empty() {
    return colorImage.empty();
}

void ImageCache::setColorImage(cv::Mat pm) {
    LOG(INFO) << "Acq ...";
    colorImage = std::move(pm);
    LOG(INFO) << "Acq over ...";
}

cv::Mat ImageCache::getColorImage() {
    return colorImage;
}

void ImageCache::setDepthData(cv::Mat *pm) {
    __del_ptr__(depthData);
    depthData = pm;
}

const cv::Mat *ImageCache::getDepthData() {
    return depthData;
}

void ImageCache::clear() {
    if (depthData) {
        delete depthData;
    }
}
