/****************************************************************************
**
** Copyright ©2019-2020 D.Jun. All rights reserved.
**
** This file is entry of the visio program.
**
****************************************************************************/

#include "GLogUtil.h"
#include "YamlUtil.h"
#include "GRuntime.h"
#include "MainWindow.h"
#include "CameraProcessor.h"

int main(int argc, char ** argv) {
    /// load configuration
    auto cfg = argc == 1 ?
            YamlUtil::create("config/visio.yml") :
            YamlUtil::create(argv[1]);
    /// initialize google log
    GLogUtil::initLog(argc, argv);
    LOG(INFO) << "visio";
    LOG(INFO) << "Copyright ©2019-2020 D.Jun. All rights reserved.";
    /// build GRuntime
    GRuntime::build();
    /// build camera processor first
    CameraProcessor::buildAndGet();
    /// build serial communicator
    SerialWrapper sw; sw.init();
    GRuntime::ref().pSer = &sw;
    /// entry main loop of windows
	return MainWindow::main(argc, argv);
}

/**
 * This function just for debugging.
 */
void jtest::prt(const uint8_t *data, int len) {
    const uint8_t *p = data;
    char buf[512]{0};
    for (auto i = 0; i < len; i++)
        sprintf(buf + 5 * i, "0x%02x ", *(p + i));
    buf[len] = '\0';
    /// Please reserve the following comment of outputting log.
    /// It will be useful when you debug serial port module.
//    LOG(INFO) << "Sending serial data: [ " << std::string(buf) << " ]";
}
