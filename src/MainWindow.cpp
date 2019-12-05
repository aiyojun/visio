//
// Created by aiyo on 2019/11/22.
//

#include "MainWindow.h"
#include <QApplication>
#include <QSplashScreen>
#include "VisionContainer.h"
#include "CameraProcessor.h"
#include <glog/logging.h>

int MainWindow::main(int argc, char **argv) {
    QApplication app(argc, argv);
    QPixmap qPixmap;
    qPixmap.load(":/color01.jpg");
//    QSplashScreen qSplashScreen(qPixmap);
//    qSplashScreen.show();
//    qSplashScreen.showMessage("prepare camera device 1 ... ...", Qt::AlignRight | Qt::AlignBottom, Qt::white);

    VisionContainer v;
//    ImageContainer v;
    v.show();
//    CameraProcessor::buildAndGet()->start();
    LOG(INFO) << "Display gui window && enter window main loop.";
    return QApplication::exec();
}