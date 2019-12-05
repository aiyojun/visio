//
// Created by aiyo on 11/23/19.
//

#ifndef VISIO_IMAGEPROCESSOR_H
#define VISIO_IMAGEPROCESSOR_H

#include <opencv2/core.hpp>
#include <librealsense2/hpp/rs_frame.hpp>
#include <comm/macro.h>

class ImageProcessor {
public:
    ImageProcessor();
    ~ImageProcessor();

    bool process(cv::Mat& processedImage, cv::Mat& areaMask, std::vector<PointsGroup>& points,
                    const cv::Mat& img, const rs2::depth_frame& depth, const struct rs2_intrinsics *intr);
    static void calculate(const cv::Rect& rect, const struct rs2_intrinsics *intr,
                          const rs2::depth_frame& depth, const cv::Mat &mask, std::vector<PointsGroup>& ps);
    static cv::Point3f pix2point(const cv::Point& p, const struct rs2_intrinsics *intr, const rs2::depth_frame& depth);
    static cv::Mat depth0proc(const cv::Mat& depth);
private:
    std::vector<int> lowerLimitOfGreen{35, 43, 46};
    std::vector<int> upperLimitOfGreen{77, 255, 255};
};


#endif //VISIO_IMAGEPROCESSOR_H
