#!/bin/bash
qt_dir=/home/${USER}/Qt5.12.0/5.12.0/gcc_64
RUNNER=${qt_dir}/bin/moc
$RUNNER src/gui/VisionContainer.h -o src/gui/moc_VisionContainer.cpp
$RUNNER src/gui/ImageContainer.h  -o src/gui/moc_ImageContainer.cpp
$RUNNER src/gui/BottomStatus.h  -o src/gui/moc_BottomStatus.cpp
$RUNNER src/gui/TopBar.h  -o src/gui/moc_TopBar.cpp
$RUNNER src/ImageCache.h  -o src/moc_ImageCache.cpp
~/Qt5.12.0/5.12.0/gcc_64/bin/rcc visio.qrc -o src/visio_rsc.cpp
