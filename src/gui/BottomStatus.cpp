//
// Created by aiyo on 11/23/19.
//

#include <CameraProcessor.h>
#include "BottomStatus.h"
#include "comm/macro.h"

BottomStatus::BottomStatus(QWidget *parent)
: QWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground, true);
    setGeometry(WND_BORDER, parent->height() - BOTTOM_HEIGHT, parent->width() - WND_BORDER * 2, BOTTOM_HEIGHT - WND_BORDER);
    pStatusInfo = new QLabel("visio status message.", this);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    pStatusInfo->setFont(font);
//    pStatusInfo->setTextFormat();
    pStatusInfo->setPalette(pe);
    pStatusInfo->setGeometry(WND_BORDER, 0, width(), BOTTOM_HEIGHT - WND_BORDER);
}

BottomStatus::~BottomStatus() {
    __del_ptr__(pStatusInfo);
}

void BottomStatus::showMessage(const std::string &msg) {
    pStatusInfo->setText(tr(msg.c_str()));
}
