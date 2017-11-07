#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define ENABLETHRESHOLD 15000
#define DISABLETHRESHOLD 14000

//PID loop initial setup values (needs tuning)
//values are (in order), target, Pnormal, Inormal, Dnormal, stopThreshold, Dstop, Imax
#define PID_PRAMS 0, 16384, 256, -4196, 6687, -16384, 2097152
//or in english: north, 0.5, 0.0078125, -0.125, 50deg/sec, -0.5, 2^22/Inormal
///would recommend tuning each peram as its own PID loop (all other prams 0)
//MACRO DEFINITIONS (need to be finished)
#define STATUS0SET gpio_set(GPIOC,GPIO13)
#define STATUS1SET gpio_set(GPIOC,GPIO14)
#define STATUS2SET gpio_set(GPIOC,GPIO15)
#define STATUS0RESET gpio_clear(GPIOC,GPIO13)
#define STATUS1RESET gpio_clear(GPIOC,GPIO14)
#define STATUS2RESET gpio_clear(GPIOC,GPIO15)

#include <stdint.h>
#include "PID.h"
#include "motor.h"
#include "sensor.h"
#include "GPS.h"
//#include "SD.h"

volatile uint32_t ms_countdown = 0;// needs to be decremented by 1(if above 0) once per ms
volatile uint32_t ms_time = 0;//should be incremented by 1 once per ms
uint32_t cycleNum = 0;

uint32_t curAlt;
int16_t curRot;
int16_t curFace;

PID_var PID1;

void init();
void shutdown();
void _delay_ms(const uint16_t delay);

int main(){
	init();
	//preflight mode
	while(1);{//wait until above threshold
		curRot = getRotation();//get the rotation every cycle
		curFace = getFacing();
		curAlt = GPS_getAlt();
		if(curAlt>ENABLETHRESHOLD)break;
		//SD_log(ms_time, curAlt, curFace, curRot, 0);
		_delay_ms(30000);//wait 30 seconds before asking again
	}
	//inflight mode
	while(1){
		curRot = getRotation();//get the rotation every cycle
		if((cycleNum&7)==0) curFace = getFacing();//get the facing every 8 cycles (due to the lower sample rate on the magnetomiter
		int16_t M1 = PID_update(&PID1, curRot, curFace);//update the PID loop (running with just 1 loop instead of 3 as they would al be the same for now)
		//SD_log(ms_time, curAlt, curFace, curRot, M1);
		motor_update(M1,M1,M1);//update the 3 motors with the newly calculated value
		_delay_ms(19);//should give 50Hz update rate, if not then adjust to fit
		cycleNum++;
		if((cycleNum&511)==0){//once every 512 cycles (25.6sec)
			curAlt=GPS_getAlt();
			if(curAlt<DISABLETHRESHOLD){//if height has dropped below threshold
				break;//leave cycle loop and go into shutdown
			}
		}
	}
	//post flight mode
	motor_update(0,0,0);//turn off all motors
	while(1){
		curRot = getRotation();//get the rotation every cycle
		if((cycleNum&7)==0) curFace = getFacing();//get the facing every 8 cycles (due to the lower sample rate on the magnetomiter
		if((cycleNum&511)==0) curAlt=GPS_getAlt();//once every 512 cycles (25.6sec) update altitude
		//SD_log(ms_time, curAlt, curFace, curRot, 0);
		_delay_ms(19);//should give 50Hz update rate, if not then adjust to fit
		cycleNum++;
	}
}

void init(){
	rcc_clock_setup_in_hsi48_out_48mhz();
	
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	
	//status Leds
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13|GPIO14|GPIO15);
	
	//systick
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(47999);//1kHz at 48MHz clock
	systick_interrupt_enable();//enable the sys_tick interrupt
	systick_counter_enable();//start the counter

	//should probably light LEDs for each part functioning
	if(sensorInit())STATUS0SET;
	if(GPS_init())STATUS1SET;
	//if(SD_init())STATUS2SET;
	motor_init()
	PID1 = PID_new(PID_PRAMS);
	_delay_ms(5000);wait 5 seconds with the LEDS on;
	STATUS0RESET;STATUS1RESET;STATUS2RESET;//turn LEDs off
}

void sys_tick_handler(void){
	if(ms_countdown)ms_countdown--;
	ms_time++;
}

void _delay_ms(const uint16_t delay){//ensures a wait of at least delay ms and no more than delay+1 ms
	ms_countdown = delay;
	while(ms_countdown);
}