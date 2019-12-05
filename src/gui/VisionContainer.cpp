//
// Created by aiyo on 2019/11/22.
//

#include "VisionContainer.h"
#include <QPalette>
#include <QtGui/QIcon>
#include "YamlUtil.h"
#include <glog/logging.h>
#include <QtGui/QPainter>
#include <CameraProcessor.h>
#include <GRuntime.h>
#include "comm/macro.h"

VisionContainer::VisionContainer(QWidget* parent)
: QWidget(parent) {
    auto cfg = YamlUtil::getYamlFile();
    setFixedSize(cfg["WINDOW"]["min_width"].as<int>(), cfg["WINDOW"]["min_height"].as<int>());
//    setMinimumSize(cfg["WINDOW"]["min_width"].as<int>(), cfg["WINDOW"]["min_height"].as<int>());
    pImageContainer = new ImageContainer(this);
//    pImageContainer2 = new ImageContainer(this);
//    LOG(INFO) << "after create image container";
    pImageContainer ->setGeometry(WND_BORDER, WND_BORDER + TOPBAR_HEIGHT,
            width() - WND_BORDER * 2, height() - WND_BORDER * 2 - TOPBAR_HEIGHT - BOTTOM_HEIGHT);
//    pImageContainer2->setGeometry(width() / 2 + WND_BORDER, WND_BORDER + TOPBAR_HEIGHT,
//            (width() - WND_BORDER * 4) / 2, height() - WND_BORDER * 2 - TOPBAR_HEIGHT - BOTTOM_HEIGHT);
//    QPalette qPalette;
//    qPalette.setBrush(QPalette::Background, QBrush(QPixmap(":/bbb.jpg")));
//    qPalette.setColor(QPalette::Background, QColor(cfg["WINDOW"]["back_color"].as<std::string>().c_str()));
//    setPalette(qPalette);
    QIcon qIcon;
    qIcon.addFile(":/icon.svg");
    setWindowIcon(qIcon);
//    LOG(INFO) << "width: " << width();
//    LOG(INFO) << "height: " << height();
//    LOG(INFO) << "pImageContainer->x: " << pImageContainer->x();
//    LOG(INFO) << "pImageContainer->y: " << pImageContainer->y();
//    LOG(INFO) << "pImageContainer->width: " << pImageContainer->width();
//    LOG(INFO) << "pImageContainer->height: " << pImageContainer->height();
    pTorBar = new TopBar(this);
    pBottomStatus = new BottomStatus(this);
    connect(pTorBar->p1, SIGNAL(clicked()), this, SLOT(startCameraThread()));
    connect(pTorBar->p2, SIGNAL(clicked()), this, SLOT(closeCameraThread()));
    connect(pTorBar->p3, SIGNAL(clicked()), this, SLOT(setCameraThreadSampleMode()));
    GRuntime::ref().pTopBar = pTorBar;
    GRuntime::ref().pStatusBar = pBottomStatus;
    GRuntime::ref().pImageCache1 = pImageContainer;
//    GRuntime::ref().pImageCache2 = pImageContainer2;
}

VisionContainer::~VisionContainer() {
    CameraProcessor::buildAndGet()->joinCameraThread();
    if (!pImageContainer) {
        delete pImageContainer;
    }
}

void VisionContainer::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.drawPixmap(QRect(0, 0, width(), height()), QPixmap(":/bbb.jpg"));
//    QWidget::paintEvent(event);
}

void VisionContainer::closeEvent(QCloseEvent *event) {
    CameraProcessor::buildAndGet()->close();
}

void VisionContainer::startCameraThread() {
    CameraProcessor::buildAndGet()->start();
}

void VisionContainer::closeCameraThread() {
    CameraProcessor::buildAndGet()->close();
}

void VisionContainer::setCameraThreadSampleMode() {
    GRuntime::ref().pStatusBar->showMessage("single sample mode, press 'link' to start sample.");
    CameraProcessor::buildAndGet()->sampleMode = true;
}