//
// Created by aiyo on 2019/11/22.
//
#include "ImageContainer.h"
#include <glog/logging.h>
#include <QtGui/QPainter>
#include <opencv2/imgcodecs.hpp>
#include <YamlUtil.h>
#include <CameraProcessor.h>
#include "comm/macro.h"
#include "ImageUtil.h"
#include <mutex>

ImageContainer::ImageContainer(QWidget *parent)
: QWidget(parent) {
    auto cfg = YamlUtil::getYamlFile();
    QPalette qPalette;
    qPalette.setColor(QPalette::Background,
            QColor(cfg["WINDOW"]["font_color"].as<std::string>().c_str()));
    setPalette(qPalette);
    setAutoFillBackground(true);
    pTempImage = new QImage();
    pTempImage->load(":/v.svg");
}

ImageContainer::~ImageContainer() {
    if (!pTempImage) {
        delete pTempImage;
    }
}

void ImageContainer::paintEvent(QPaintEvent *event) {
    lock.lock();
    QPainter p(this);
//    p.begin(this);
    if (cache.empty()) {
        p.drawImage(QRect((width() - pTempImage->width()) / 2, (height() - pTempImage->height()) / 2,
                        pTempImage->width(), pTempImage->height()), *pTempImage);
    } else {
        p.drawImage(QRect(0, 0, width(), height()),
                ImageUtil::convertMat2QImage(cache[0]));
    }
//    p.end();
    lock.unlock();
//    QWidget::paintEvent(event);
}

void ImageContainer::updateScreen() {
    update();
}

void ImageContainer::cacheBatchImage(const std::vector<cv::Mat>& c) {
    cache = c;
    update();
//    repaint();
}
