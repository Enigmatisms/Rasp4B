# Rasp4B
Sensor integration with face detection and opencv3 support.
### 2021.1.4 / 2020.1.12
---
【树莓派嵌入式实验代码库】

- 基本传感器诸如：雨滴探测器，超声传感器，陀螺仪，IR控制模块
- 摄像头：Sargo 4K Ultra HD 人脸识别【可扩展】
- 多线程监听红外事件
- C/C++混编项目

---

#### 使用到的库

- wiringPi for C
- LIRC (Linux Infrared Remote Control)
- OpenCV version 3.3.1

- CMake

---

#### 配置方法

- 请一定先按照系统的类别进行源的配置。本项目使用Raspbian buster，由于系统默认源是南京大学的（nju，不好用），请换为清华源：

```shell
sudo gedit /etc/apt/sources.list
```

1. 替换源为：

```
deb http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ buster main contrib non-free rpi
deb-src http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ buster main contrib non-free rpi
```

2. 此后执行：

```shell
sudo apt-get update
sudo apt-get upgrade
```

​		可能会遇到没有公钥的问题（并且公钥服务器也连接不上，搜索ubuntu的公钥服务器，下载对应公钥）。

3. 更换系统源：

```shell
sudo gedit /etc/apt/sources.list.d/raspi.list
```

​		替换内容为：

```
deb http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ buster main ui
deb-src http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ buster main ui
```

- CMake

```shell
sudo apt-get install cmake
```

- OpenCV 3.3.1 使用Github源码编译。首先需要安装相关库支持，比如libgtk2.0-dev等等，请自行搜索。OpenCV源码编译：在文件夹下（无需opencv_contrib）

```shell
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
# 此行说明release编译，并将make install的include / lib文件夹放置在/usr/local下
make -j4
sudo make install
sudo ldconfig
```

​		OpenCV的编译完成，请测试是否能正常重启。本人因为错误的源以及错误的配置曾经两次冲刷系统。

- LIRC：按照指导书上的安装方式进行安装，只是CMake配置的时候需要找一下在何处。

---

#### CMake

```cmake
target_link_libraries(Task		# 动态链接库的配置
    pthread						# 多线程
    lirc_client					# LIRC库
    ${WIRINGPI_LIBRARIES}		# wiringPi
    ${OpenCV_LIBS}				# opencv库
    ${LIRC_LIBRARIES}			# LIRC
)
```

- 如果找不到LIRC库，可以使用：

```cmake
set(LIRC_LIBS "directory to lirc libs")
set(LIRC_INCLUDE_DIR "directory to lirc include path")
```

​		来设置库加载路径以及头文件路径。

---

#### 说明

​		人脸识别需要执行，需要一个USB摄像头直接连接在树莓派上。并且，需要根据文件【src/control.cc】修改级联分类器的加载路径：

```c++
const std::string cascadeName = "/home/pi/FaceData/haarcascade/haarcascade_frontalface_alt2.xml";
const std::string nestedCascadeName = "/home/pi/FaceData/haarcascade/haarcascade_eye_tree_eyeglasses.xml";
const std::string cascadeName2 = "/home/pi/FaceData/haarcascade/aGest.xml";
```

