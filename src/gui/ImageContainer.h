//
// Created by aiyo on 2019/11/22.
//

#ifndef VISIO_IMAGECONTAINER_H
#define VISIO_IMAGECONTAINER_H


#include <QWidget>
#include <QtWidgets/QPushButton>
#include <ImageCache.h>
#include <QtGui/QPainter>
#include <mutex>

class ImageContainer : public QWidget {
    Q_OBJECT
public:
    explicit ImageContainer(QWidget* parent = nullptr);
    virtual ~ImageContainer();
    void cacheBatchImage(const std::vector<cv::Mat>& c);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    std::mutex lock;
    QImage* pTempImage = nullptr;
    std::vector<cv::Mat> cache;
//    QPainter p;
//    ImageCache imageCache;
//    std::vector<cv::Mat> depthImageCache;
private slots:
    void updateScreen();
};


#endif //VISIO_IMAGECONTAINER_H
