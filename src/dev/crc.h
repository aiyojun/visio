//
// Created by aiyo on 2019/11/27.
//

#ifndef VISIO_CRC_H
#define VISIO_CRC_H


#include <stdint-gcc.h>

class crc {
public:
    static uint16_t crc16(const uint8_t* p, int len);
private:
    static const uint16_t crc16tab[256];
};


#endif //VISIO_CRC_H
