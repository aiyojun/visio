#!/bin/bash

auth=$(id -u)
if [ $auth -ne 0 ]
then
	echo "\033[31m--> Run the file as root.\033[0m"
	exit 1
fi

apt-get update
echo ""
echo "-----------------------------------"
echo ""
apt install -y python3 python3-pip
pip3 install opencv-python

echo ""
echo "-----------------------------------"
echo ""
echo 'deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial main' | tee /etc/apt/sources.list.d/realsense-public.list
apt-key adv --keyserver keys.gnupg.net --recv-key 6F3EFCDE
apt-get update
apt-get install -y librealsense2-dkms librealsense2-utils
apt-get install -y librealsense2-dev librealsense2-dbg

echo ""
echo "-----------------------------------"
echo ""
cd /opt && git clone https://github.com/aa1608450902/visio.git && cd visio
cp share/visio.desktop /usr/share/applications