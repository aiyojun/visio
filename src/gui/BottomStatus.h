//
// Created by aiyo on 11/23/19.
//

#ifndef VISIO_BOTTOMSTATUS_H
#define VISIO_BOTTOMSTATUS_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

class BottomStatus : public QWidget {
    Q_OBJECT
public:
    BottomStatus(QWidget* parent = nullptr);
    virtual ~BottomStatus();
    void showMessage(const std::string& msg);
protected:

private:
    QLabel* pStatusInfo = nullptr;
};


#endif //VISIO_BOTTOMSTATUS_H
