#define ABS(x) ((x)<0 ? (-x) : (x))

typedef struct{
	int16_t target;
	int16_t Pnormal;
	int16_t Inormal;
	int16_t Dnormal;
	int32_t integrator;
	int16_t stopThreshold;
	int16_t Dstop;
	int32_t Imax;
} PID_var;

//fixed point multiply where 1=2^14
int16_t fixMul(int16_t a, int16_t b);

//iterate the PID loop once, updating the integrator and returning the new output value
//if the rate of rotation is above the threshold it will use the Dstop value in a damping loop to slow rotation
int16_t PID_update(PID_var* var, int16_t Rot, int16_t Face){
	if(ABS(Rot)>var->stopThreshold){
		//use stop D loop
		//integrator frozen so dont update it
		return safeAdd(fixMul(Rot,var->Dstop), fixMul32(var->integrator,var->Inormal));
	}else{
		//use normal PID loop
		int16_t dif = subBearing(var->target,Face);//difference in bearing taking into account the continuity between int_max and int_min
		var->integrator = clamp32(var->integrator+dif, var->Imax);
		return safeAdd(fixMul(dif,var->Pnormal),//P term
				safeAdd(fixMul32(var->integrator,var->Inormal),//Iterm
				fixMul(Rot,var->Dnormal)));//D term, all added safely to avoid overflow (maybe a little slow though)
	}
	return 0;
}

//genarate a new PID_var for the provided settings (inital integrator value 0)
//P,I and D values are fixed point multiplies where 2^14=1, 2^13=0.5
PID_var PID_new(int16_t target, int16_t Pnormal, int16_t Inormal, int16_t Dnormal,
		int16_t stopThreshold, int16_t Dstop, int32_t Imax){
	return (PID_var){target,Pnormal,Inormal,Dnormal,
			0,stopThreshold,Dstop,Imax};
}

int16_t fixMul32(int32_t a, int16_t b){
	int32_t c = (int32_t)a*b;//do the mult (may overflow if integrator limit is too high
	int16_t out = c>>14//shift it back
	if(c&(1<<13))out++;//do rounding
	return out;
}

int16_t fixMul(int16_t a, int16_t b){
	int32_t c = (int32_t)a*b;//do the mult
	int16_t out = c>>14//shift it back
	if(c&(1<<13))out++;//do rounding
	return out;
}

int32_t safeAdd(int16_t a, int16_t b){
	int32_t c = (int32_t)a+b;
	if(c>INT16_MAX)return INT16_MAX;
	if(c<INT16_MIN)return INT16_MIN;
	return (int16_t)c;
}

int32_t clamp32(int32_t a, int16_t max){
	if(a>max)return max;
	if(a<(-max))return -max;
	return a;
}

int16_t subBearing(int16_t a, int16_t b){
	return a-b;
}