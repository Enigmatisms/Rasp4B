#ifndef __CTRL_HPP__
#define __CTRL_HPP__

#include <deque>
#include <vector>
#include <iostream>
#include <pcf8591.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
extern "C"{
    #include "./imu.h"
	#include "./ultra.h"
	#include "./lcd.h"
}

#define		PCF     120
#define		DOpin	0

const char ultra_dis[16] = "Distance:";
const char ultra_warn[2][16] = {"< Warning ! >", "Obs detected."};
const int BuzzerPin = 2;

class Ctrl{
public:
    Ctrl();
    ~Ctrl();
public:
    void ultraSonic(bool do_alarm);
    void rainingDetect();
    void photoDetect(){;}
    void cvQuiry(){;}
    void odometry(){;}
    void accDetect();
    void detectAndDraw(
        cv::Mat &img,
        double scale = 1.0
    );
    void joyStick();
    void drawPos(cv::Mat& src);
private:
    bool buzzer;
    int raining;
    char to_write[16];
    cv::CascadeClassifier cascade;
    cv::CascadeClassifier nestedCascade;
    std::deque<cv::Point> dq;
    int posx;
    int posy;
    int reset_cnt;
};

#endif //__CTRL_HPP__