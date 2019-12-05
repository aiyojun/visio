//
// Created by aiyo on 2019/11/22.
//

#include "GLogUtil.h"

using namespace google;

void GLogUtil::initLog(int argc, char **argv) {
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);
}
