#include "../include/control.hpp"

const std::string cascadeName = "/home/pi/FaceData/haarcascade/haarcascade_frontalface_alt2.xml";
const std::string nestedCascadeName = "/home/pi/FaceData/haarcascade/haarcascade_eye_tree_eyeglasses.xml";
const std::string cascadeName2 = "/home/pi/FaceData/haarcascade/aGest.xml";

const cv::Scalar colors[] = {
    CV_RGB(0, 0, 255),
    CV_RGB(0, 128, 255),
    CV_RGB(0, 255, 255),
    CV_RGB(0, 255, 0),
    CV_RGB(255, 128, 0),
    CV_RGB(255, 255, 0),
    CV_RGB(255, 0, 0),
    CV_RGB(255, 0, 255)
};

Ctrl::Ctrl(){
    fd = wiringPiI2CSetup(LCDAddr);
    fd2 = wiringPiI2CSetup (0x68);
    wiringPiI2CWriteReg8 (fd2,0x6B,0x00);//disable sleep mode 
    printf("IMU 0x6B=%X\n",wiringPiI2CReadReg8 (fd2,0x6B));
	init();
	if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
	}

	ultraInit();
    pinMode(2,  OUTPUT);
    pcf8591Setup(PCF, 0x48);
	pinMode(DOpin, INPUT);
    raining = 0;
    buzzer = false;
    cascade.load(cascadeName);
    nestedCascade.load(nestedCascadeName);
    posx = 320;
    posy = 240;
    reset_cnt = 0;
}

Ctrl::~Ctrl(){
    clear();
}

void Ctrl::rainingDetect(){
    int analogVal = analogRead(PCF + 0);
	int tmp = digitalRead(DOpin);
	if (tmp == 0){
        writeStr(0, 0, "It is raining.");
        writeStr(0, 1, "I hate raining");
    }
    else{
        writeStr(0, 0, "Not raining.");
        writeStr(0, 1, "That is good.");
    }
	delay (200);
}

void Ctrl::joyStick(){
	int x, y, b;
	x = analogRead(PCF + 3);
	y = analogRead(PCF + 2);
	b = analogRead(PCF + 1);
	if (y < 100){
		if (posy > 0){
            posy -= 2;
        }		// up
    }
	if (x > 250){
		if (posy < 480){
            posy += 2;
        }
    }
	
	if (x < 100){
        if (posx > 0){
            posx -= 2;
        }
    }
		
	if (y > 250){
        if (posx < 640){
            posx += 2;
        }
    }
		
	if (b < 100){
        reset_cnt++;
        if (reset_cnt > 10){
            posx = 320;
            posy = 240;
        }
    }
    else{
        reset_cnt = 0;
    }
}

void Ctrl::drawPos(cv::Mat& src){
    cv::rectangle(src, cv::Rect(0, 0, 640, 480), cv::Scalar(0, 0, 0), -1);
    cv::circle(src, cv::Point(posx, posy), 5, cv::Scalar(255, 255, 255), -1);
    cv::imshow("joyCar", src);
}

void Ctrl::ultraSonic(bool do_alarm){
    float dis;
    if (do_alarm == false){
        if (buzzer ==true){
            buzzer = false;
            digitalWrite(BuzzerPin, HIGH);
        }
	    dis = disMeasure();
        writeStr(0, 0, to_write);
	    snprintf(to_write, 16, "%0.2f cm", dis);
	    writeStr(1, 1, to_write);
    }
    else{
        dis = disMeasure();
        //loading
        digitalWrite(BuzzerPin, HIGH);
        buzzer = false;

        if (dis > 50.0){
            writeStr(0, 0, ultra_dis);
	        snprintf(to_write, 16, "%0.2f cm", dis);
	        writeStr(1, 1, to_write);
        }
        else{
            if (buzzer == false){
                digitalWrite(BuzzerPin, LOW);
                buzzer = true;
            }
            writeStr(0, 0, ultra_warn[0]);
            writeStr(1, 1, ultra_warn[1]);
        }
    }
    
	delay(1000);
}

void Ctrl::detectAndDraw(
    cv::Mat &img,
    double scale
){
    int i = 0;
    double t = 0;
    std::vector<cv::Rect> faces;

    cv::Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);

    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
    cv::equalizeHist(smallImg, smallImg);
    cascade.detectMultiScale(smallImg, faces,
        1.1, 2, 0 //| CV_HAAR_FIND_BIGGEST_OBJECT
                    //|CV_HAAR_DO_ROUGH_SEARCH
                    | CV_HAAR_SCALE_IMAGE,
        cv::Size(30, 30));
    for (std::vector<cv::Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
    {
        cv::Mat smallImgROI;
        std::vector<cv::Rect> nestedObjects;
        cv::Point center;
        cv::Scalar color = colors[i % 8];
        int radius;
        center.x = cvRound((r->x + r->width * 0.5) * scale);
        center.y = cvRound((r->y + r->height * 0.5) * scale);
        radius = cvRound((r->width + r->height) * 0.25 * scale);
        cv::rectangle(img, cv::Point(center.x + radius, center.y + radius), cv::Point(center.x - radius, center.y - radius), color, 3, 8, 0); //circle( img, center, radius, color, 3, 8, 0 );
        if (nestedCascade.empty())
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
            1.1, 2, 0
                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                        //|CV_HAAR_DO_ROUGH_SEARCH
                        //|CV_HAAR_DO_CANNY_PRUNING
                        | CV_HAAR_SCALE_IMAGE,
            cv::Size(30, 30));
        for (std::vector<cv::Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
        {
            center.x = cvRound((r->x + nr->x + nr->width * 0.5) * scale);
            center.y = cvRound((r->y + nr->y + nr->height * 0.5) * scale);
            radius = cvRound((nr->width + nr->height) * 0.25 * scale);
            // rectangle(img,Point(center.x+radius,center.y+radius),Point(center.x-radius,center.y-radius),color,1.5,8,0);
            cv::circle(img, center, radius, color, 2, 8, 0);
        }
    }
    cv::imshow("result", img);
    int people = faces.size();
    char tmp_str[16];
    snprintf(tmp_str, 16, "Number: %d", people);
    writeStr(0, 0, tmp_str);
    writeStr(0, 1, "Hold upright.");    
}

void Ctrl::accDetect(){
    double acclX = readWord2c(0x3B) / 16384.0;
    double acclY = readWord2c(0x3D) / 16384.0;
    double acclZ = readWord2c(0x3F) / 16384.0;
    double angX = get_x_rotation(acclX, acclY, acclZ);
    double angY = get_y_rotation(acclX, acclY, acclZ);
    snprintf(to_write, 16, "X Y:%0.1f, %0.1f", angX, angY);
    writeStr(0, 0, to_write);
    buzzer = false;
    digitalWrite(BuzzerPin, HIGH);
    if (abs(angX) > 45.0){
        if (buzzer == false){
            digitalWrite(BuzzerPin, LOW);
            buzzer = true;
        }
        writeStr(0, 1, "Warning: Tilt!");
    }
    else{
        writeStr(0, 1, "That is good.");
    }
    delay(500);
}