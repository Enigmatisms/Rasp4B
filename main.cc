
#include <iostream>
#include <thread>
#include <mutex>
#include "include/control.hpp"
extern "C"{
	#include "include/ircontrol.h"
}

char *code;
uchar status, old_status;

void requiry(){
	while(1){
		lirc_nextcode(&code);
		status = statusMap(key(code));
		printf("Current status is %d\n", status);
	}
}

int main(int argc, char* argv[]){
	status = 0;
	cv::VideoCapture cap(0);
	cap.open(0);
	if (cap.isOpened() == false){
		std::cerr << "No camera feed.\n";
	}
	cv::Mat frame;
	cv::Mat joy;
	frame.create(cv::Size(640, 480), CV_8UC3);
	joy.create(cv::Size(640, 480), CV_8UC3);
	Ctrl ctrl;
	std::mutex mtx;
	struct lirc_config *config;
	int buttonTimer = millis();
	char *c;
	if(lirc_init("lirc",1)==-1)
		exit(EXIT_FAILURE);

	if(lirc_readconfig(NULL,&config,NULL)==0)
	{
		std::thread th(requiry);
		th.detach();
		while(1)
		{
			switch(status){
				case 0:{
					writeStr(3, 0, "Idle mode.");
					writeStr(0, 1, "Nothing is done.");
					delay(200);
					break;
				}
				case 1:{
					ctrl.rainingDetect();
					break;
				}
				case 2:{
					ctrl.ultraSonic(true);
					break;
				}
				case 3:{
					ctrl.accDetect();
					break;
				}
				case 4:{
					cap.read(frame);
        			if (frame.empty() == true){
        			    break;
        			}
        			ctrl.detectAndDraw(frame);
        			char key = cv::waitKey(1);
        			if (key == 27){
        			    break;
        			}
					break;
				}
				case 5:{
					ctrl.joyStick();
					ctrl.drawPos(joy);
					char key = cv::waitKey(1);
        			if (key == 27){
        			    break;
        			}
					break;
				}
				default:
					;
			}
			if (status != 4){
				clear();
			}
			mtx.lock();
			if (old_status != status){
				cv::destroyAllWindows();
			}
			if (status == 4 && old_status != status){
				clear();
			}
			old_status = status;
			if(code==NULL) {
				mtx.unlock();
				continue;
			}
			free(code);
			mtx.unlock();
		}
		lirc_freeconfig(config);
	}
	lirc_deinit();
	clear();
    return 0;
}