cmake_minimum_required(VERSION 2.8.0)

project(visio)

set(EXECUTABLE_OUTPUT_PATH bin)

# set(CMAKE_PREFIX_PATH /home/$ENV{USER}/Qt5.12.0/5.12.0/gcc_64)

add_compile_options(-std=c++11)

message("-- user： $ENV{USER}")

message("-- CMAKE_PREFIX_PATH： ${CMAKE_PREFIX_PATH}")

find_package(Qt5Widgets)

include_directories(
		src
		src/gui
		src/proc
		/home/$ENV{USER}/git-repo/yaml-cpp/include
		/usr/include
		/usr/local/include
		/usr/local/include/opencv4
)

link_directories(
		/usr/local/lib
		/home/aiyo/git-repo/yaml-cpp/build
)

add_executable(visio
		src/main.cpp
		src/visio_rsc.cpp
		src/ImageUtil.cpp src/ImageUtil.h
		src/MainWindow.cpp src/MainWindow.h
		src/GLogUtil.cpp src/GLogUtil.h
		src/gui/VisionContainer.cpp src/gui/VisionContainer.h
		src/gui/moc_VisionContainer.cpp
		src/gui/moc_ImageContainer.cpp src/gui/ImageContainer.cpp src/gui/ImageContainer.h
		src/YamlUtil.cpp src/YamlUtil.h
		src/proc/ImageProcessor.cpp src/proc/ImageProcessor.h
		src/comm/macro.h
		src/ImageCache.cpp src/ImageCache.h src/moc_ImageCache.cpp
		src/CameraProcessor.cpp src/CameraProcessor.h
		src/gui/TopBar.cpp src/gui/TopBar.h src/gui/moc_TopBar.cpp
		src/gui/BottomStatus.cpp src/gui/BottomStatus.h src/gui/moc_BottomStatus.cpp
		src/dev/SerialCommunicator.cpp src/dev/SerialCommunicator.h
		src/GRuntime.cpp src/GRuntime.h
		src/dev/crc.cpp src/dev/crc.h
		src/dev/SerialWrapper.cpp src/dev/SerialWrapper.h
		)

target_link_libraries(visio
		stdc++
		Qt5::Widgets
		glog
		yaml-cpp
		opencv_core
		opencv_highgui
		opencv_imgcodecs
		opencv_imgproc
		realsense2
		pthread
)
