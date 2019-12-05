//
// Created by aiyo on 11/23/19.
//

#ifndef VISIO_IMAGECACHE_H
#define VISIO_IMAGECACHE_H


#include <opencv2/core/mat.hpp>
#include <queue>
#include <QObject>

class ImageCache : public QObject {
    Q_OBJECT
public:
    ImageCache();
    ~ImageCache();
    void setColorImage(cv::Mat pm);
    cv::Mat getColorImage();
    void setDepthData(cv::Mat *pm);
    const cv::Mat* getDepthData();
    bool empty();
    void clear();
    void notifyWindow() {
        emit updateScreen();
    }
private:
    cv::Mat colorImage;
    cv::Mat *depthData = nullptr;
signals:
    void updateScreen();
};


#endif //VISIO_IMAGECACHE_H
