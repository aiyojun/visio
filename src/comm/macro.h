//
// Created by aiyo on 11/23/19.
//

#pragma once

#define WND_BORDER 5
#define TOPBAR_HEIGHT 38
#define BOTTOM_HEIGHT 30

#define __del_ptr__(ptr) if (ptr) delete ptr

#define BUTTON_STYLE \
"QPushButton{background:transparent;color:white;}"\
"QPushButton:hover{border:none;color:rgb(10,124,235);}"\
"QPushButton:pressed{border:none;color:rgb(10,124,235);}"

#include <opencv2/core.hpp>
#include <glog/logging.h>

namespace jtest {
    void prt(const uint8_t *data, int len);
}

struct PointsGroup {
    PointsGroup(size_t i, size_t ip, float h, std::vector<cv::Point3f> g) {
        index = i; ipkgs = ip; height = h; gp = std::move(g);
    }
    size_t index;
    size_t lpkgs = 0;
    size_t ipkgs;
    float height;
    std::vector<cv::Point3f> gp;
};

