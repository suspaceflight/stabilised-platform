#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <inttypes.h>

#define I2CH_RCC RCC_I2C1
#define I2CH I2C1
#define I2CH_PORT GPIOB
#define I2CH_AFn GPIO_AF1
#define I2CH_SCL_PIN GPIO8
#define I2CH_SDA_PIN GPIO9

void i2ch_init(void);
void i2ch_Wreg(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t i2ch_Rreg(uint8_t addr, uint8_t reg);

void i2ch_init(void){
	rcc_periph_clock_enable(I2CH_RCC);
	gpio_mode_setup(I2CH_PORT, I2CH_AFn, GPIO_PUPD_NONE, I2CH_SCL_PIN|I2CH_SDA_PIN);
	gpio_set_af(I2CH_PORT, I2CH_AFn, I2CH_SCL_PIN|I2CH_SDA_PIN);

	I2C_CR1(I2CH) = 0;//turns a bunch of stuff off, currently disabled
	//prescaler = 1, low period = 0x13, high period = 0xf, hold time = 2, setup = 4
	I2C_TIMINGR(I2CH) = (1<<28) | (4<<20) | (2<<16) | (0xf<<8) | 0x13;
	I2C_CR1(I2CH) |= I2C_CR1_PE;//enable
}

void i2ch_Wreg(uint8_t addr, uint8_t reg, uint8_t data){
	while((I2C_ISR(I2CH) & I2C_ISR_BUSY) != 0);//wait until not busy
	while((I2C_CR2(I2CH) & I2C_CR2_START));//wait until ready to start
	I2C_CR2(I2CH) = (I2C_CR2(I2CH) & ~(0xFF<<16)) | (2 << I2C_CR2_NBYTES_SHIFT);//set two bytes to transfer
	I2C_CR2(I2CH) = (I2C_CR2(I2CH) & ~(0x3FF)) | ((addr));//set the address
	I2C_CR2(I2CH) &= ~I2C_CR2_RD_WRN;//say that this is a write
	I2C_CR2(I2CH) &= ~I2C_CR2_AUTOEND;//enable auto end
	I2C_CR2(I2CH) |= I2C_CR2_START; //send a start

	int wait = 1;
	while(wait){
		if((I2C_ISR(I2CH) & I2C_ISR_TXIS) != 0){//wait until the previous buffer is clear and ready for more data
			wait = false;
		}
		while((I2C_ISR(I2CH) & I2C_ISR_NACKF) != 0); /* Some error */
	}
	I2C_TXDR(I2CH) = reg;//send the requested reg

	int wait = 1;
	while(wait){
		if((I2C_ISR(I2CH) & I2C_ISR_TXIS) != 0){//wait until the previous buffer is clear and ready for more data
			wait = false;
		}
		while((I2C_ISR(I2CH) & I2C_ISR_NACKF) != 0); /* Some error */
	}
	I2C_TXDR(I2CH) = data;//send the requested reg
}
uint8_t i2ch_Rreg(uint8_t addr, uint8_t reg){
	while((I2C_ISR(I2CH) & I2C_ISR_BUSY) != 0);//wait until not busy
	while((I2C_CR2(I2CH) & I2C_CR2_START));//wait until ready to start
	I2C_CR2(I2CH) = (I2C_CR2(I2CH) & ~(0xFF<<16)) | (1 << I2C_CR2_NBYTES_SHIFT);//set one byte to transfer
	I2C_CR2(I2CH) = (I2C_CR2(I2CH) & ~(0x3FF)) | ((addr));//set the address
	I2C_CR2(I2CH) &= ~I2C_CR2_RD_WRN;//say that this is a write
	I2C_CR2(I2CH) &= ~I2C_CR2_AUTOEND;//dont autoend
	I2C_CR2(I2CH) |= I2C_CR2_START; //send a start

	int wait = 1;
	while(wait) {
		if((I2C_ISR(I2CH) & I2C_ISR_TXIS) != 0){//wait until the previous buffer is clear and ready for more data
			wait = false;
		}
		while ((I2C_ISR(I2CH) & I2C_ISR_NACKF) != 0); /* Some error */
	}
	I2C_TXDR(I2CH) = reg;//send the requested reg

	while((I2C_CR2(I2CH) & I2C_CR2_START));//wait until ready to restart
	I2C_CR2(I2CH) = (I2C_CR2(I2CH) & ~(0xFF<<16)) | (1 << I2C_CR2_NBYTES_SHIFT);//set one byte to transfer
	//dont need to reset address
	I2C_CR2(I2CH) |= I2C_CR2_RD_WRN;//say that its a read
	I2C_CR2(I2CH) &= ~I2C_CR2_AUTOEND;//enable auto end
	I2C_CR2(I2CH) |= I2C_CR2_START; //send a start

	while((I2C_ISR(i2c) & I2C_ISR_RXNE) != 0);
	return I2C_RXDR(i2c) & 0xff;
}
