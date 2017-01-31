//should set up any pins used, and possibly do a test GPS_getAlt() to confirm its working
//return 1 if working, return 0 if not
int GPS_init(){
	return 1;
}

//query the GPS micro-controller for the current altitude
//return altitude in meters
//should take less than 20ms to complete
uint32_t GPS_getAlt(){
	return 0;
}