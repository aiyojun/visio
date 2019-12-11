//
// Created by root on 2019/12/10.
//
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <glog/logging.h>
using namespace std;

struct PointsGroup {
    PointsGroup(size_t i, size_t ip, float h, std::vector<cv::Point3f> g) {
        index = i; ipkgs = ip; height = h; gp = std::move(g);
    }
    size_t index;
    size_t lpkgs = 0;
    size_t ipkgs;
    float height;
    int heightp = 0;
    std::vector<cv::Point3f> gp;
};

void calculate(const cv::Rect &rect, const cv::Mat &mask,
               std::vector<PointsGroup>& pg);

int main(int argc, char* argv[]) {
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);
    cv::Mat mask = (cv::Mat_<uint8_t>(10, 10) <<
            0, 0, 0, 0, 0, /**/ 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, /**/ 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, /**/ 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, /**/ 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, /**/ 1, 1, 1, 0, 0,

            0, 0, 0, 0, 1, /**/ 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, /**/ 1, 1, 1, 0, 0,
            0, 0, 0, 0, 1, /**/ 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, /**/ 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, /**/ 0, 0, 0, 0, 0
            );
    std::vector<PointsGroup> pg;
    calculate(cv::Rect(3, 2, 5, 6), mask, pg);
    return 0;
}

cv::Point3f pix2point(const cv::Point& p) {
    return {static_cast<float>(p.x),
            static_cast<float>(p.y), 12.12};
}

void calculate(const cv::Rect &rect, const cv::Mat &mask,
                               std::vector<PointsGroup>& pg) {
    LOG(INFO) << "  mask.type()   : " << mask.type();
    cv::Point pp1(rect.x, rect.y), pp2(rect.x + rect.width - 1, rect.y),
            pp3(rect.x, rect.y + rect.height - 1), pp4(rect.x + rect.width - 1, rect.y + rect.height - 1);
    cv::Point3f p1(pix2point(pp1)), p2(pix2point(pp2)),
            p3(pix2point(pp3)), p4(pix2point(pp4));
    auto dlong = std::fabs(p2.x - p1.x); // dlong: mm
    auto dheight = std::fabs(p3.y - p1.y);
    // ratio: mm / 10 mm
    auto plong = std::abs(pp2.x - pp1.x);
    auto pheight = std::fabs(pp3.y - pp1.y);
    auto pix_per_mm_x = plong / dlong;
    auto pix_per_mm_y = pheight / dheight;
    int countx = int(dlong / 1);
    int county = int(dheight / 1);
    LOG(INFO) << ">> The following is identified area information:";
    LOG(INFO) << "-------------------------------";
    LOG(INFO) << "  width  (pix)  : " << plong;
    LOG(INFO) << "  height (pix)  : " << pheight;
    LOG(INFO) << "  width  (mm)   : " << dlong;
    LOG(INFO) << "  height (mm)   : " << dheight;
//    LOG(INFO) << "  vertex     top-left depth (mm) : " << pix2point(pp1, intr, depth).z;
//    LOG(INFO) << "  vertex    top-right depth (mm) : " << pix2point(pp2, intr, depth).z;
//    LOG(INFO) << "  vertex  bottom-left depth (mm) : " << pix2point(pp3, intr, depth).z;
//    LOG(INFO) << "  vertex bottom-right depth (mm) : " << pix2point(pp4, intr, depth).z;
    LOG(INFO) << "-------------------------------";
    size_t index = 1;
    for (int j = 0; j <= county; j++) {
        bool hasLineData = false;
        int ipkg = 0;
        std::vector<cv::Point3f> pv;
        int delta_pix_y = int(float(j) * pix_per_mm_y * 1);
        for (int i = 0; i <= countx; i++) {
            int delta_pix_x = int(float(i) * pix_per_mm_x * 1);
            cv::Point pp(pp1.x + delta_pix_x, pp1.y + delta_pix_y);
            if (mask.at<uint8_t>(pp) != 0) {
            LOG(INFO) << "X : " << pp1.x + delta_pix_x << "\t Y : " << pp1.y + delta_pix_y;
                if (pv.size() == 3) {
                    hasLineData = true;
                    pg.emplace_back(PointsGroup(index, ++ipkg, pv.front().y, pv));
                    pg.back().heightp = pp1.y + delta_pix_y;
                    pv.clear();
                }
                auto fp = pix2point(pp);
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
            outp += "(" + std::to_string(point.x) + ", " + std::to_string(point.y) +  "),";
        }
        if (outp[outp.length() - 1] == ',') outp = outp.substr(0, outp.length() - 1);
        LOG(INFO) << outp;
    }
}
