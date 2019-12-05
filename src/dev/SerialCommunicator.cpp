//
// Created by aiyo on 11/24/19.
//

#include "SerialCommunicator.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <glog/logging.h>

#include <utility>
#include <GRuntime.h>
#include <YamlUtil.h>
#include "comm/macro.h"

SerialCommunicator::SerialCommunicator(std::string d, int b)
: device(std::move(d)), baudRate(b) {

}

void SerialCommunicator::connect() {
    auto cfg = YamlUtil::getYamlFile();
    bool isop = cfg["SERIAL"]["open"].as<bool>();
    if (!isop) {
        prepared = false;
        return;
    }
    if (prepared) {
        return;
    }
    device = cfg["SERIAL"]["dev"].as<std::string>();
    baudRate = cfg["SERIAL"]["baud_rate"].as<int>();
    serialPortFd = open(device.data(), O_RDWR);
    if (serialPortFd < 0) {
        LOG(ERROR) << "Open serial port failed!";
        return;
    }
    LOG(INFO) << "open serial port success, id: " << serialPortFd;
    struct termios oldttys1{}, newttys1{};
    if (tcgetattr(serialPortFd, &oldttys1) != 0) ;
    bzero(&newttys1, sizeof(newttys1));
    newttys1.c_cc[VMIN]  = 12;
    newttys1.c_cc[VTIME] = 60;

    newttys1.c_cflag |= (CLOCAL | CREAD);
    newttys1.c_cflag &= ~CSIZE;
//    int cs = 8, stop = 1;
//    char validation = 'N';
    /// cs setting
    newttys1.c_cflag |= CS8;
    /// odd/even/without validation setting
    newttys1.c_cflag &= ~PARENB;
    if (brmp.find(baudRate) != brmp.end()) {
        cfsetispeed(&newttys1, brmp.at(baudRate));
        cfsetospeed(&newttys1, brmp.at(baudRate));
    } else {
        cfsetispeed(&newttys1, B115200);
        cfsetospeed(&newttys1, B115200);
    }
    cfsetispeed(&newttys1, B115200);
    cfsetospeed(&newttys1, B115200);
    /// stop bit setting
    newttys1.c_cflag &= ~CSTOPB;
    if ((tcsetattr(serialPortFd, TCSANOW, &newttys1))!=0)
        return;
    prepared = true;
    LOG(INFO) << "prepare serial port success, id: " << serialPortFd;
}

SerialCommunicator::~SerialCommunicator() {
    if (prepared) {
        if (serialPortFd > -1) {
            close(serialPortFd);
            serialPortFd = -1;
        }
        prepared = false;
    }
}

void SerialCommunicator::sends(uint8_t *p, int len) {
    if (!prepared) {
        GRuntime::ref().pStatusBar->showMessage("make sure your serial port ok!");
        return;
    }
    jtest::prt(p, len);
    if (write(serialPortFd, p, len) < len) {
        LOG(WARNING) << "serial port transmit unknown error";
    }
}

void SerialCommunicator::reads(uint8_t *p, int len) {
    if (!prepared) {
        GRuntime::ref().pStatusBar->showMessage("make sure your serial port ok!");
        return;
    }/*
    if (len != 64) {
        LOG(INFO) << "serial port command must be 64 byte, otherwise contact manager to change setting.";
        return;
    }*/
    jtest::prt(p, len);
    if (read(serialPortFd, p, len) < 0) {
        LOG(WARNING) << "unknown number byte of serial port command, please check.";
    }
}

void SerialCommunicator::set(const std::string &dev, int baud) {
    device = dev;
    baudRate = baud;
}
