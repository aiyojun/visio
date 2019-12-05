//
// Created by aiyo on 2019/11/27.
//

#include "SerialWrapper.h"
#include <glog/logging.h>
#include <GRuntime.h>
#include <comm/macro.h>
#include "crc.h"

void SerialWrapper::init() {
    sc.connect();
}

void SerialWrapper::sendto(const uint8_t *p, int len, int pkgi, int n) {
    if (len > 56) {
        LOG(ERROR) << "Transmit data overflow, data size must lower 64 byte, now : " << len;
        return;
    }
    uint8_t buf[64]{};
    buf[0] = 0xff;
    buf[1] = 0xf0;
    buf[2] = uint8_t(n);
    buf[3] = uint8_t(pkgi);
    buf[4] = 0x02;
    buf[5] = 0x3a;
    for (int i = 0; i < len; i++) {
        buf[i + 6] = p[i];
    }
    for (int i = len; i < 56; i++) {
        buf[i + 6] = 0x00;
    }
    uint16_t crc_code = crc::crc16(buf, 62);
    buf[62] = uint8_t(crc_code >> 8);
    buf[63] = uint8_t(crc_code);
    sc.sends(buf, 64);
}

void SerialWrapper::readto(uint8_t *p, int len) {
    sc.reads(p, len);
}

void SerialWrapper::sendPoints(const std::vector<PointsGroup> &points) {
    int pkgi = 0;
    for (const auto & gp : points) {
        std::vector<uint8_t> prefix{0x00, 0x01, uint8_t(gp.gp.size()), uint8_t(gp.index)};
        push_point_height(prefix, gp.height);
        for (const auto & point : gp.gp) {
            push_point(prefix, point);
        }
        sendto(prefix.data(), prefix.size(), gp.ipkgs, gp.lpkgs);
        uint8_t buf[64]{}; readto(buf, 64);
    }
}

void SerialWrapper::push_point(std::vector<uint8_t>& buf, cv::Point3f point) {
    auto *pz = (uint8_t *)&point.z;
    auto *px = (uint8_t *)&point.x;
    buf.emplace_back(pz[0]);
    buf.emplace_back(pz[1]);
    buf.emplace_back(pz[2]);
    buf.emplace_back(pz[3]);
    buf.emplace_back(px[0]);
    buf.emplace_back(px[1]);
    buf.emplace_back(px[2]);
    buf.emplace_back(px[3]);
}

void SerialWrapper::packagePoints(const std::vector<cv::Point3f> &points, std::vector<std::vector<uint8_t>> &groups) {
    std::vector<uint8_t> buf;
    float curr_height = points.front().y;
    for (const auto & point : points) {
        if (std::abs(point.y - curr_height) > 15) {
            if (!buf.empty()) {
                groups.emplace_back(buf);
                buf.clear();
            }
            curr_height = point.y;
            push_point_height(buf, curr_height);
            push_point(buf, point);
        } else {
            if (buf.empty()) {
            	push_point_height(buf, curr_height);
                push_point(buf, point);
            } else if (buf.size() == 52) {
                groups.emplace_back(buf);
                buf.clear();
                push_point_height(buf, curr_height);
                push_point(buf, point);
            } else if (buf.size() < 52) {
                push_point(buf, point);
            } else {
                LOG(ERROR) << "";
            }
        }
    }
    if (!buf.empty()) {
        groups.emplace_back(buf);
    }
    for (const auto& bb : groups) {
        jtest::prt(bb.data(), bb.size());
    }
}

void SerialWrapper::push_point_height(std::vector<uint8_t> &buf, float y) {
    auto *py = (uint8_t *)&y;
    buf.emplace_back(py[0]);
    buf.emplace_back(py[1]);
    buf.emplace_back(py[2]);
    buf.emplace_back(py[3]);
}

void SerialWrapper::flush() {
    if (!cacher.empty()) {
        // LOG(INFO) << "send green area points data:";
        // std::string s = "\n";
        // int count = 0;
        // for (const auto& point : cacher) {
        //     count++;
        //     s += "(" + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.x) + "),";
        //     if (count % 10 == 0) {
        //         s += "\n";
        //     }
        // }
        // LOG(INFO) << s;
        sendPoints(cacher);
    } else {
        LOG(INFO) << "without serial data in cache.";
    }
    cacher.clear();
}

void SerialWrapper::cache(const std::vector<PointsGroup> &points) {
    cacher = points;
    LOG(INFO) << "cache green area points.";
}
