//
// Created by aiyo on 11/24/19.
//

#include "GRuntime.h"
#include <glog/logging.h>

GRuntime* GRuntime::self = nullptr;

GRuntime* GRuntime::build() {
    if (!self) {
        self = new GRuntime();
    }
    return self;
}

GRuntime &GRuntime::ref() {
//    if (!self->pTopBar || !self->pStatusBar) {
//        LOG(ERROR) << "please initialize GRuntime";
//    }
    return *self;
}
