//
// Created by aiyo on 2019/11/27.
//

#ifndef VISIO_SERIALWRAPPER_H
#define VISIO_SERIALWRAPPER_H

#include "SerialCommunicator.h"
#include <vector>
#include <opencv2/core.hpp>
#include <comm/macro.h>

class SerialWrapper {
public:
    SerialWrapper() = default;
    void init();
    void sendto(const uint8_t *p, int len, int pkgi = 0, int n = 1);
    void readto(uint8_t *p, int len);
    void sendPoints(const std::vector<PointsGroup>& points);
    static void push_point(std::vector<uint8_t>& buf, cv::Point3f point);
    static void push_point_height(std::vector<uint8_t>& buf, float h);
    static void packagePoints(const std::vector<cv::Point3f>& points, std::vector<std::vector<uint8_t>>& groups);
    void cache(const std::vector<PointsGroup>& points);
    void flush();
private:
    std::vector<PointsGroup> cacher;
    SerialCommunicator sc;
};


#endif //VISIO_SERIALWRAPPER_H
