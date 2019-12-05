//
// Created by aiyo on 11/23/19.
//

#include <QtGui/QPainter>
#include "TopBar.h"
#include "comm/macro.h"

#define BT_WIDTH 50
#define BT_HEIGHT 25

TopBar::TopBar(QWidget* parent): QWidget(parent) {
    setGeometry(WND_BORDER, WND_BORDER, parent->width() - WND_BORDER * 2, TOPBAR_HEIGHT);
//    QPalette qPalette;
//    qPalette.setColor(QPalette::Background, QColor("#000000"));
//    setAutoFillBackground(true);
//    setPalette(qPalette);
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    plogo = new QImage(":/logo.png");
    p1 = new QPushButton("link", this);
    p1->setFont(font);
    p1->setFlat(true);
    p1->setGeometry(width() - (WND_BORDER + BT_WIDTH), (TOPBAR_HEIGHT - BT_HEIGHT) / 2, BT_WIDTH, BT_HEIGHT);
    p1->setStyleSheet(BUTTON_STYLE);
    p2 = new QPushButton("stop", this);
    p2->setFlat(true);
    p2->setFont(font);
    p2->setStyleSheet(BUTTON_STYLE);
    p2->setGeometry(width() - (WND_BORDER * 2 + BT_WIDTH * 2), (TOPBAR_HEIGHT - BT_HEIGHT) / 2, BT_WIDTH, BT_HEIGHT);
    p3 = new QPushButton("sample", this);
    p3->setFlat(true);
    p3->setFont(font);
    p3->setStyleSheet(BUTTON_STYLE);
    p3->setGeometry(width() - (WND_BORDER * 3 + BT_WIDTH * 2 + BT_WIDTH * 1.5), (TOPBAR_HEIGHT - BT_HEIGHT) / 2,
            BT_WIDTH * 1.5, BT_HEIGHT);
}

TopBar::~TopBar() {
    __del_ptr__(p1);
    __del_ptr__(p2);
    __del_ptr__(p3);
}

void TopBar::paintEvent(QPaintEvent *event) {
//    QPainter p(this);
//    //QPainter::Antialiasing |
//    p.setRenderHints(QPainter::SmoothPixmapTransform);
//    auto nlogo = plogo->scaled(TOPBAR_HEIGHT * 3, TOPBAR_HEIGHT);
//    p.drawImage(QRect(0, 0, TOPBAR_HEIGHT * 3, TOPBAR_HEIGHT), nlogo);
    QWidget::paintEvent(event);
}

