# ECE435 Lab 7 IoT Car

## Steps for Distance detection with CAN to IP Gateway with two Arduinos
1. Flash the Arduino with the sensor sensor using sonar.ino
2. Flash the Arduino with the CAN2IP shield using CAN2IP.ino

## Steps for TensorFlow Object Detection on the Raspberry Pi
### 1. Update the Raspberry Pi
```sh
sudo apt-get update
sudo apt-get dist-upgrade
```

### 2. Install TensorFlow
```sh
mkdir tf
cd tf
wget https://github.com/lhelontra/tensorflow-on-arm/releases/download/v1.8.0/tensorflow-1.8.0-cp35-none-linux_armv7l.whl
sudo pip3 install /home/pi/tf/tensorflow-1.8.0-cp35-none-linux_armv7l.whl
sudo apt-get install libatlas-base-dev
sudo pip3 install pillow lxml jupyter matplotlib cython
sudo apt-get install python-tk
```

### 3. Install OpenCV
```sh
sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libxvidcore-dev libx264-dev
sudo apt-get install qt4-dev-tools
pip3 install opencv-python
```

### 4. Compile and Install Protobuf
```sh
sudo apt-get install autoconf automake libtool curl
wget https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-all-3.5.1.tar.gz
tar -zxvf protobuf-all-3.5.1.tar.gz
cd protobuf-3.5.1
./configure
make # takes a long time
make check 
sudo make install
cd python
export LD_LIBRARY_PATH=../src/.libs
python3 setup.py build --cpp_implementation 
python3 setup.py test --cpp_implementation
sudo python3 setup.py install --cpp_implementation
export PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=cpp
export PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION_VERSION=3
sudo ldconfig
protoc # to confirm that you installed it
sudo reboot now # for some reason the Pi needs to be rebooted for tf to work
```

### 5. Set up TensorFlow Directory Structure and PYTHONPATH Variable
```sh
mkdir tensorflow1
cd tensorflow1
git clone --recurse-submodules https://github.com/tensorflow/models.git
sudo nano ~/.bashrc
export PYTHONPATH=$PYTHONPATH:/home/pi/tensorflow1/models/research:/home/pi/tensorflow1/models/research/slim
cd /home/pi/tensorflow1/models/research
protoc object_detection/protos/*.proto --python_out=.
cd /home/pi/tensorflow1/models/research/object_detection
wget http://download.tensorflow.org/models/object_detection/ssdlite_mobilenet_v2_coco_2018_05_09.tar.gz
tar -xzvf ssdlite_mobilenet_v2_coco_2018_05_09.tar.gz
```

### 6. Detect Objects!
```sh
python3 camera.py 
```

#### Credits
Setup and code credits to https://github.com/EdjeElectronics/TensorFlow-Object-Detection-on-the-Raspberry-Pi
