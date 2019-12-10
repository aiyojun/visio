//
// Created by aiyo on 11/23/19.
//

#include "ImageProcessor.h"
#include <opencv2/imgproc.hpp>
#include <glog/logging.h>
#include <librealsense2/rsutil.h>
#include <YamlUtil.h>
#include <GRuntime.h>

ImageProcessor::ImageProcessor() {
    auto cfg = YamlUtil::getYamlFile();
    lowerLimitOfGreen = cfg["CAMERA"]["lower_green"].as<std::vector<int>>();
    upperLimitOfGreen = cfg["CAMERA"]["upper_green"].as<std::vector<int>>();
}

ImageProcessor::~ImageProcessor() = default;

bool ImageProcessor::process(cv::Mat& processedImage, cv::Mat& areaMask, std::vector<PointsGroup>& points, const cv::Mat& img,
        const rs2::depth_frame& depth, const struct rs2_intrinsics *intr) {
    cv::Mat dst, mask;
    cv::cvtColor(img, dst, cv::COLOR_BGR2HSV);
    cv::inRange(dst, lowerLimitOfGreen, upperLimitOfGreen, mask);
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_8UC1);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RetrievalModes::RETR_EXTERNAL,
            cv::ContourApproximationModes::CHAIN_APPROX_NONE);
    img.copyTo(processedImage);
    if (contours.size() != 1) {
        LOG(WARNING) << "Not only one contour! size: " << contours.size();
        return false;
    }
    /**
     * important parameter: rect
     */
    cv::Rect rect = cv::boundingRect(contours[0]);
    calculate(rect, intr, depth, mask, points);
    if (!points.empty()) {
        GRuntime::ref().pSer->cache(points);
    }
    /// draw rect in the original image
    cv::rectangle(processedImage, rect, cv::Scalar(255, 0, 0), 2);
    cv::drawContours(processedImage, contours, 0, cv::Scalar(0, 255, 0), cv::FILLED);
    return true;
}


void ImageProcessor::calculate(const cv::Rect &rect, const struct rs2_intrinsics *intr,
                               const rs2::depth_frame &depth, const cv::Mat &mask,
                               std::vector<PointsGroup>& pg) {
    cv::Point pp1(rect.x, rect.y), pp2(rect.x + rect.width - 1, rect.y),
            pp3(rect.x, rect.y + rect.height - 1), pp4(rect.x + rect.width - 1, rect.y + rect.height - 1);
    cv::Point3f p1(pix2point(pp1, intr, depth)), p2(pix2point(pp2, intr, depth)),
            p3(pix2point(pp3, intr, depth)), p4(pix2point(pp4, intr, depth));
    auto dlong = std::fabs(p2.x - p1.x); // dlong: mm
    auto dheight = std::fabs(p3.y - p1.y);
    // ratio: mm / 10 mm
    auto plong = std::abs(pp2.x - pp1.x);
    auto pheight = std::fabs(pp3.y - pp1.y);
    auto pix_per_mm_x = plong / dlong;
    auto pix_per_mm_y = pheight / dheight;
    int countx = int(dlong / 10);
    int county = int(dheight / 30);
    LOG(INFO) << ">> The following is identified area information:";
    LOG(INFO) << "-------------------------------";
    LOG(INFO) << "  width  (pix)  : " << plong;
    LOG(INFO) << "  height (pix)  : " << pheight;
    LOG(INFO) << "  width  (mm)   : " << dlong;
    LOG(INFO) << "  height (mm)   : " << dheight;
    LOG(INFO) << "  vertex     top-left depth (mm) : " << pix2point(pp1, intr, depth).z;
    LOG(INFO) << "  vertex    top-right depth (mm) : " << pix2point(pp2, intr, depth).z;
    LOG(INFO) << "  vertex  bottom-left depth (mm) : " << pix2point(pp3, intr, depth).z;
    LOG(INFO) << "  vertex bottom-right depth (mm) : " << pix2point(pp4, intr, depth).z;
    LOG(INFO) << "-------------------------------";
    size_t index = 1;
    for (int j = 0; j <= county; j++) {
        bool hasLineData = false;
        int ipkg = 0;
        std::vector<cv::Point3f> pv;
        int delta_pix_y = int(float(j) * pix_per_mm_y * 30);
        for (int i = 0; i <= countx; i++) {
            int delta_pix_x = int(float(i) * pix_per_mm_x * 10);
            cv::Point pp(pp1.x + delta_pix_x, pp1.y + delta_pix_y);
            if (mask.at<uint8_t>(pp) != 0) {
                if (pv.size() == 6) {
                    hasLineData = true;
                    pg.emplace_back(PointsGroup(index, ++ipkg, pv.front().y, pv));
                    pg.back().heightp = pp1.y + delta_pix_y;
                    pv.clear();
                }
                auto fp = pix2point(pp, intr, depth);
                pv.emplace_back(fp);
            }
        }
        if (!pv.empty()) {
            hasLineData = true;
            pg.emplace_back(PointsGroup(index, ++ipkg, pv.front().y, pv));
            pg.back().heightp = pp1.y + delta_pix_y;
            pv.clear();
        }
        for (int i = ipkg; i > 0; i--) {
            pg[pg.size() - i].lpkgs = ipkg;
        }
        if (hasLineData) {
            ++index;
        }
    }
    LOG(INFO) << ">> package points : ";
    for (const auto& package: pg) {
        std::string outp = "  height (pix) : " + std::to_string(package.heightp) + "\t line points: ";
        for (const auto& point: package.gp) {
            outp += "(" + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + "),";
        }
        if (outp[outp.length() - 1] == ',') outp = outp.substr(0, outp.length() - 1);
        LOG(INFO) << outp;
    }
}

cv::Point3f ImageProcessor::pix2point(const cv::Point& p, const struct rs2_intrinsics *intr, const rs2::depth_frame& depth) {
    float point[3]{};
    float pixel[2]{float(p.x), float(p.y)};

    rs2_deproject_pixel_to_point(point, intr, pixel, depth.get_distance(p.x, p.y));
    auto cfg = YamlUtil::getYamlFile();
    return {point[0] * 1000 * cfg["MODEL"]["x_ratio"].as<float>(),
            point[1] * 1000 * cfg["MODEL"]["y_ratio"].as<float>(), point[2] * 1000};
}

cv::Mat ImageProcessor::depth0proc(const cv::Mat &depth) {
    int countp = 0;
    cv::Mat r;
    depth.copyTo(r);
    for (int j = 0; j < depth.rows; j++) {
        for (int i = 0; i < depth.cols; i++) {
            if (depth.at<float>(j, i) == 0 &&
                    (j > 0 && j < depth.rows - 1) &&
                    (i > 0 && i < depth.cols - 1)) {
                int count = 0;
                float   d1 = depth.at<float>(j - 1, i),
                        d2 = depth.at<float>(j + 1, i),
                        d3 = depth.at<float>(j, i - 1),
                        d4 = depth.at<float>(j, i + 1);
                float   d5 = depth.at<float>(j - 1, i - 1),
                        d6 = depth.at<float>(j + 1, i + 1),
                        d7 = depth.at<float>(j - 1, i + 1),
                        d8 = depth.at<float>(j + 1, i - 1);
                if (d1 > 0) count++;
                if (d2 > 0) count++;
                if (d3 > 0) count++;
                if (d4 > 0) count++;
                if (d5 > 0) count++;
                if (d6 > 0) count++;
                if (d7 > 0) count++;
                if (d8 > 0) count++;
                if (count >= 4) {
                    r.at<float>(j, i) = float((d1 + d2 + d3 + d4 + d5 + d6 + d7 + d8) / count);
                    countp++;
                }
            }
        }
    }
    LOG(INFO) << "3D points processing ( " << countp << " ).";
    return r;
}


