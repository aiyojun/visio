//
// Created by aiyo on 2019/11/21.
//

#ifndef JVISION_IMAGEUTIL_H
#define JVISION_IMAGEUTIL_H

#include <QImage>
#include <opencv2/core.hpp>
#include <librealsense2/rs.hpp>

class ImageUtil {
public:
    static QImage convertMat2QImage(const cv::Mat& m);
    static cv::Mat convertRealsense2Mat(const rs2::frame& f, int type = CV_8UC3);
    static void convertPixmap2PointGraph(const cv::Mat& m, const rs2_intrinsics& intrinsics);
};


#endif //JVISION_IMAGEUTIL_H
