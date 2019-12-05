//
// Created by aiyo on 11/24/19.
//

#ifndef VISIO_SERIALCOMMUNICATOR_H
#define VISIO_SERIALCOMMUNICATOR_H

#include <string>
#include <map>
#include <termios.h>

class SerialCommunicator {
public:
    /// send api, however, we must change
    /// the pixmap to the real 3-D location
    /// by measuring ms unit distance.
    SerialCommunicator(std::string d = "/dev/ttyS0", int b = 115200);
    ~SerialCommunicator();
    void sends(uint8_t *p, int len = 64);
    void reads(uint8_t *p, int len = 64);
    void connect();
    void set(const std::string& dev, int baud);
private:
    /// baudrate setting
    const std::map<int, int> brmp = {
            {115200, B115200},
            {230400, B230400},
            {460800, B460800},
            {500000, B500000},
            {576000, B576000},
            {921600, B921600},
            {1000000, B1000000},
            {1152000, B1152000},
            {1500000, B1500000},
            {2000000, B2000000},
            {2500000, B2500000},
            {3000000, B3000000},
            {3500000, B3500000},
            {4000000, B4000000},
    };
    bool prepared = false;
    std::string device;
    int serialPortFd = -1;
    int baudRate;
};


#endif //VISIO_SERIALCOMMUNICATOR_H
