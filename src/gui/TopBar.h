//
// Created by aiyo on 11/23/19.
//

#ifndef VISIO_TOPBAR_H
#define VISIO_TOPBAR_H


#include <QtWidgets/QWidget>
#include <QPushButton>

class TopBar : public QWidget {
    Q_OBJECT
public:
    TopBar(QWidget* parent = nullptr);
    virtual ~TopBar();
    QPushButton *p1 = nullptr;
    QPushButton *p2 = nullptr;
    QPushButton *p3 = nullptr;
protected:
    void paintEvent(QPaintEvent *event);
private:
    QImage *plogo = nullptr;
};


#endif //VISIO_TOPBAR_H
