//
// Created by aiyo on 2019/11/22.
//

#ifndef VISIO_VISIONCONTAINER_H
#define VISIO_VISIONCONTAINER_H

#include <QWidget>
#include "ImageContainer.h"
#include "TopBar.h"
#include "BottomStatus.h"

class VisionContainer : public QWidget {
    Q_OBJECT
public:
    explicit VisionContainer(QWidget* parent = nullptr);
    ~VisionContainer() override;
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
private:
    ImageContainer *pImageContainer = nullptr;
//    ImageContainer *pImageContainer2 = nullptr;
    BottomStatus *pBottomStatus = nullptr;
    TopBar* pTorBar = nullptr;
private slots:
    void startCameraThread();
    void closeCameraThread();
    void setCameraThreadSampleMode();
};


#endif //VISIO_VISIONCONTAINER_H
