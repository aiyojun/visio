//
// Created by aiyo on 11/24/19.
//

#ifndef VISIO_GRUNTIME_H
#define VISIO_GRUNTIME_H


#include <gui/TopBar.h>
#include <gui/BottomStatus.h>
#include <dev/SerialWrapper.h>
#include "CameraProcessor.h"

class GRuntime {
public:
    static GRuntime* build();
    static GRuntime& ref();
    TopBar* pTopBar = nullptr;
    BottomStatus* pStatusBar = nullptr;
    CameraProcessor* pCam = nullptr;
    ImageContainer* pImageCache1 = nullptr;
    SerialWrapper* pSer = nullptr;
//    ImageContainer* pImageCache2 = nullptr;
private:
    GRuntime() = default;
    static GRuntime *self;
};


#endif //VISIO_GRUNTIME_H
