#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <math.h>
#include "i2ch.h"
#include "atanC.h"

#define GYRO_ADDR 0b11010100
#define GYRO_REGZ 0x2A
#define GYRO_REG_CTRL 0x20

#define MAG_ADDR  0b00111010
#define MAG_REGX 0x08
#define MAG_REGY 0x0A
#define MAG_REG_CTRL 1F

#define PI 3.14159265359
#define RAD2INT16 ((double)INT16_MAX/PI)

//initialise and setup both the gryo and the magnetometer
int sensorInit(){
	i2ch_init();//does all the nasty i2c init
	
	//gyro init
	if(i2ch_Rreg(GYRO_ADDR,0x0F)!=0b11010111)return 0;//could not read 'who_am_i' register
	i2ch_Wreg(GYRO_ADDR,GYRO_REG_CTRL,0b10001010);//50Hz sample rate, y axis only and turned on
	i2ch_Wreg(GYRO_ADDR,GYRO_REG_CTRL+3,0b00000000);//245deg/sec limit, use 0b00010000 for 500deg/sec
	//should already be in bypass mode
	
	//mag init
	if(i2ch_Rreg(MAG_ADDR,0x0F)!=0b01001001)return 0;//could not read 'who_am_i' register
	i2ch_Wreg(MAG_ADDR,MAG_REG_CTRL+5,0b01100100);//high res, 6.25Hz sample rate
	i2ch_Wreg(MAG_ADDR,MAG_REG_CTRL+6,0b00000000);//+-2gauss (should be fine for compass, but may act odd near magnets)
	i2ch_Wreg(MAG_ADDR,MAG_REG_CTRL+7,0b00000000);//unfiltered, continuous conversion
	
	return 1;
}

//query the gyro and return a value for the rotation about the y axis
//(may be different axis if sensor is mounted differently)
//output value should correlate as int16_max = largest possible sensor value
//and int16_min = largest negative sensor value
int16_t getRotation(){
	return (int16_t)(((uint16_t)i2ch_Rreg(GYRO_ADDR, GYRO_REGX+1)<<8)+i2ch_Rreg(GYRO_ADDR, GYRO_REGX));
}

//querry the magnetometer and return a value for magnetic compass bearing
//0 = north, int16_max = +180deg, int16_min = -180deg
int16_t getFacing(){
	int16_t x;
	int16_t y;
	x = (int16_t)(((uint16_t)i2ch_Rreg(MAG_ADDR, MAG_REGX+1)<<8)+i2ch_Rreg(MAG_ADDR, MAG_REGX));
	y = (int16_t)(((uint16_t)i2ch_Rreg(MAG_ADDR, MAG_REGY+1)<<8)+i2ch_Rreg(MAG_ADDR, MAG_REGY));

	int flipX=0;
	int flipY=0;
	int swapXY=0;

	//method involves working out what octant we are in first to avoid any loss of sign or division by 0
	if(x<0){//make x positive
		flipX=1;
		x=-x;
	}
	if(y<0){//make y positive
		flipY=1;
		y=-y;
	}
	if(x>y){//more than 45deg, swap to avoid div0 or near div0
		int16_t temp = x;
		x=y;
		y=temp;
		swapXY=1;
	}
	//garenteed to be positive and less than pi/8
	int16_t out = atanC(x,y);
	if(swapXY)out = ((INT16_MAX/2)+1) - out;
	if(flipY)out+=((INT16_MAX/2)+1);//add pi/2
	if(flipX)out=-out;//flip the sign
	
	return out;
}