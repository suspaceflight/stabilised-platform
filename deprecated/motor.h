#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

//set up the 6 pwms for the 3 motors and set them all to 0% duty cycle
void motor_init(){
	//enable timer clocks
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_clock_enable(RCC_TIM3);
	
	gpio_mode_setup(GPIOA, GPIO_AF2, GPIO_PUPD_NONE, GPIO0|GPIO1|GPIO2|GPIO3|GPIO6|GPIO7);
	gpio_set_af(GPIOA, GPIO_AF2, GPIO0|GPIO1|GPIO2|GPIO3|GPIO6|GPIO7);
	
	TIM2_CR1 = 0;//0 clock div, edge aligned, upcounter
	TIM3_CR1 = 0;
	TIM2_PSC = 0;//no prescaler
	TIM3_PSC = 0;
	TIM2_ARR = 2048;//23437.5Hz when on 48MHz
	TIM3_ARR = 2048;//should be fast enough for the hbridge to take it as analogue
	
	TIM3_EGR = 1;//continuous mode?
	
	//set ch 1-4 to pwm mode, with compare change on reload
	TIM2_CCMR1 = TIM_CCMR1_OC1M_PWM1|TIM_CCMR1_OC1PE | TIM_CCMR1_OC2M_PWM1|TIM_CCMR1_OC2PE;
	TIM2_CCMR2 = TIM_CCMR2_OC3M_PWM1|TIM_CCMR2_OC3PE | TIM_CCMR2_OC4M_PWM1|TIM_CCMR2_OC4PE;
	//set ch 1-2 to pwm mode, with compare change on reload
	TIM3_CCMR1 = TIM_CCMR1_OC1M_PWM1|TIM_CCMR1_OC1PE | TIM_CCMR1_OC2M_PWM1|TIM_CCMR1_OC2PE;
	
	TIM2_CCER |= TIM_CCER_CC1E|TIM_CCER_CC2E|TIM_CCER_CC3E|TIM_CCER_CC4E;//set outputs enabled for ch 1-4
	TIM3_CCER |= TIM_CCER_CC1E|TIM_CCER_CC2E;//set outputs enabled for ch 1-2
	
	TIM2_CCR1 = 0;//set all outputs to 0 duty cycle
	TIM2_CCR2 = 0;
	TIM2_CCR3 = 0;
	TIM2_CCR4 = 0;
	TIM3_CCR1 = 0;
	TIM3_CCR2 = 0;
	
	TIM2_CR1 |= TIM_CR1_ARPE|TIM_CR1_CEN;
	TIM3_CR1 |= TIM_CR1_ARPE|TIM_CR1_CEN;
}

//depending on whether each motor input is positive or negative, set the relevant pwms to a duty cycle based on the input value compared to int16_max
void motor_update(int16_t motor1, int16_t motor2, int16_t motor3){
	uint16_t m1 = (motor1>0)?(motor1):(-motor1);
	uint16_t m2 = (motor2>0)?(motor2):(-motor2);
	uint16_t m3 = (motor3>0)?(motor3):(-motor3);
	m1 = m1>>4;//max of 32767 becomes max of 2047
	m2 = m2>>4;
	m3 = m3>>4;
	if(motor1>0){TIM2_CCR1=m1;TIM2_CCR2=0;}else{TIM2_CCR1=0;TIM2_CCR2=m1;}
	if(motor2>0){TIM2_CCR3=m2;TIM2_CCR4=0;}else{TIM2_CCR3=0;TIM2_CCR4=m2;}
	if(motor3>0){TIM3_CCR1=m3;TIM3_CCR2=0;}else{TIM3_CCR1=0;TIM3_CCR2=m3;}
}