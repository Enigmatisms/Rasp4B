#include "../include/ircontrol.h"
#define IDLE 0
#define RAIN 1
#define ULTRA 2
#define TILT 3
#define FACE 4
#define ODOM 5

uchar color[3] = {0xff, 0xff, 0xff};
uchar Lv[3]    = {0xff, 0x44, 0x00};

char *keymap[21] ={
	" KEY_CHANNELDOWN ",
	" KEY_CHANNEL ",
	" KEY_CHANNELUP ",
	" KEY_PREVIOUS ",
	" KEY_NEXT ",
	" KEY_PLAYPAUSE ",
	" KEY_VOLUMEDOWN ",
	" KEY_VOLUMEUP ",
	" KEY_EQUAL ",
	" KEY_NUMERIC_0 ",
	" BTN_0 ",
	" BTN_1 ",
	" KEY_NUMERIC_1 ",
	" KEY_NUMERIC_2 ",
	" KEY_NUMERIC_3 ",
	" KEY_NUMERIC_4 ",
	" KEY_NUMERIC_5 ",
	" KEY_NUMERIC_6 ",
	" KEY_NUMERIC_7 ",
	" KEY_NUMERIC_8 ",
	" KEY_NUMERIC_9 "};

void ledInit(void)
{
	softPwmCreate(LedPinRed,  0, 100);
	softPwmCreate(LedPinGreen,0, 100);
	softPwmCreate(LedPinBlue, 0, 100);
}

void ledColorSet()
{
	softPwmWrite(LedPinRed,   color[0]);
	softPwmWrite(LedPinGreen, color[1]);
	softPwmWrite(LedPinBlue,  color[2]);
}

int key(char *code){
	int i;
	int num;
	for (i=0; i<21; i++){
		if (strstr(code, keymap[i])){
			num = i;
		}
	}
	return num + 1;
}

int statusMap(int i){
	switch(i){
		case 1: return RAIN; // printf("Red OFF\n"); break;
		case 2: return ULTRA; // printf("Light Red\n"); break;
		case 3: return TILT; // printf("Dark Red\n"); break;
		case 4: return FACE; // printf("Green OFF\n"); break;
		case 5: return ODOM; // printf("Light Green\n"); break;
		default: return IDLE;
		// case 6: color[1] = Lv[2]; // printf("Dark Green\n"); break;
		// case 7: color[2] = Lv[0]; // printf("Blue OFF\n"); break;
		// case 8: color[2] = Lv[1]; // printf("Light Blue\n"); break;
		// case 9: color[2] = Lv[2]; // printf("Dark Green\n"); break;
	}
}