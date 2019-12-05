//
// Created by aiyo on 2019/11/21.
//

#include "ImageUtil.h"
#include <exception>
#include <glog/logging.h>
#include <librealsense2/rsutil.h>

QImage ImageUtil::convertMat2QImage(const cv::Mat &m) {
    if (m.type() == CV_8UC1) {
//        LOG(INFO) << "CV_8UC1 ---";
        QImage qImage(m.cols, m.rows, QImage::Format_Indexed8);
        qImage.setColorCount(256);
        for(int i = 0; i < 256; i++) {
            qImage.setColor(i, qRgb(i, i, i));
        }
        uchar *pSrc = m.data;
        for(int row = 0; row < m.rows; row ++)
        {
            uchar *pDest = qImage.scanLine(row);
            memcpy(pDest, pSrc, m.cols);
            pSrc += m.step;
        }
        return qImage;
    } else if (m.type() == CV_8UC3) {
//        LOG(INFO) << "CV_8UC3 ---";
        const auto *pSrc = (const uchar*)m.data;
        QImage image(pSrc, m.cols, m.rows, m.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    } else {
        LOG(ERROR) << "unknown image format ---";
        throw std::runtime_error("unknown image format");
    }
}

cv::Mat ImageUtil::convertRealsense2Mat(const rs2::frame &f, int type) {
    return cv::Mat(cv::Size(640, 480), type, (void*)f.get_data(), cv::Mat::AUTO_STEP);
}

void ImageUtil::convertPixmap2PointGraph(const cv::Mat &m, const rs2_intrinsics &intrinsics) {
//    rs2_deproject_pixel_to_point();
}
