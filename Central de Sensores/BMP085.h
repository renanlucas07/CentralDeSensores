
/*
 * BMP085.h
 *
 * Created: 21/12/2017 11:00:27
 * Author: Renan Lucas
 */ 


#ifndef BMP085_H_
#define BMP085_H_

#include <stdio.h>
#include <math.h>
#include "i2cmaster.h"

// CALIBRATION COEFFICIENTS		// MUST BE READ FROM THE SENSOR
#define AC1 0xAA
#define AC2 0xAC
#define AC3 0xAE
#define AC4 0xB0
#define AC5 0xB2
#define AC6 0xB4
#define B1  0xB6
#define B2  0xB8
#define MB  0xBA
#define MC  0xBC
#define MD  0xBE

#define I2CADDR			  0xEE

#define ULTRALOWPOWER	  0
#define STANDARD		  1
#define HIGHRES			  2
#define ULTRAHIGHRES	  3
#define CONTROL           0xF4 
#define DATA			  0xF6
#define READTEMPCMD       0x2E
#define READPRESSURECMD   0x34

uint8_t oversampling;

int16_t ac1,ac2,ac3,b1,b2,mb,mc,md;
uint16_t ac4,ac5,ac6;

uint8_t begin(uint8_t mode);
uint16_t readRawTemperature(void);
int readTemperature(void);
uint32_t readRawPressure(void);
int32_t readPressure(void);
float readAbsAltitude(void);

// Reads the constants and Returns 0 if succeeds, 1 if fails
uint8_t begin(uint8_t mode) {
	oversampling = mode;
	i2cInit();
	// 
	if(i2cStart(I2CADDR+I2C_WRITE)){
		serialString("\nAddress registering failed\n");
		return 1;
	}
	i2cWrite(AC1);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	ac1 = i2cReadAck();
	ac1 <<= 8;
	ac1 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(AC2);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	ac2 = i2cReadAck();
	ac2 <<= 8;
	ac2 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(AC3);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	ac3 = i2cReadAck();
	ac3 <<= 8;
	ac3 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(AC4);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	ac4 = i2cReadAck();
	ac4 <<= 8;
	ac4 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(AC5);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	ac5 = i2cReadAck();
	ac5 <<= 8;
	ac5 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(AC6);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	ac6 = i2cReadAck();
	ac6 <<= 8;
	ac6 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(B1);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	b1 = i2cReadAck();
	b1 <<= 8;
	b1 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(B2);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	b2 = i2cReadAck();
	b2 <<= 8;
	b2 |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(MB);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	mb = i2cReadAck();
	mb <<= 8;
	mb |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(MC);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	mc = i2cReadAck();
	mc <<= 8;
	mc |= i2cReadNack();
	i2cStop();
	
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(MD);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	md = i2cReadAck();
	md <<= 8;
	md |= i2cReadNack();
	i2cStop();
	/*
	serialString ("\nAC1 = %d\n",ac1);
	serialSendInt()
	serialString ("\nAC2 = %d\n",ac2);
	serialSendInt()
	serialString ("\nAC3 = %d\n",ac3);
	serialSendInt()
	serialString ("\nAC4 = %u\n",ac4);
	serialSendInt()
	serialString ("\nAC5 = %u\n",ac5);
	serialString ("\nAC6 = %u\n",ac6);
	serialString ("\nB1 = %d\n",b1);
	serialString ("\nB2 = %d\n",b2);
	serialString ("\nMB = %d\n",mb);
	serialString ("\nMC = %d\n",mc);
	serialString ("\nMD = %d\n",md);*/
	
	return 0;
}

// Reads the raw temperature value
uint16_t readRawTemperature(void) {
	uint16_t UT;
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(CONTROL);
	i2cWrite(READTEMPCMD);
	i2cStop();
	_delay_us(4500);
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(DATA);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	UT = i2cReadAck();
	UT <<= 8;
	UT |= i2cReadNack();
	return UT;
}

// Reads the true temperature value in oC
int readTemperature(void) {
	uint16_t UT;
	int32_t X1,X2;
	int temp;
	UT = readRawTemperature();
	//serialString("\nUT = %u",UT);
	X1 = ((UT - (int32_t)ac6) * ((int32_t)ac5) >> 15);
	X2 = (((int32_t)mc << 11) / (X1 + (int32_t)md));
	//serialString("\nx1 = %ld, x2= %ld, B5 = %ld",X1, X2,X1+X2);
	//serialString("\nRaw Temp = %ld",((X1 + X2 + 8) >> 4));
	temp = ((X1 + X2 + 8) >> 4);
	//temp /= 10.0;
	return temp;
} 

// Reads the raw pressure value
uint32_t readRawPressure(void) {
	uint32_t UP;
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(CONTROL);
	i2cWrite(READPRESSURECMD + (oversampling << 6));
	i2cStop();
	switch (oversampling) {
		case 0:
			_delay_us(4500);
			break;
		case 1:
			_delay_us(7500);
			break;
		case 2:
			_delay_us(13500);
			break;
		case 3:
			_delay_us(25500);
			break;
		default:
			break;
	}
	i2cStart(I2CADDR+I2C_WRITE);
	i2cWrite(DATA);
	i2cStop();
	i2cStart(I2CADDR+I2C_READ);
	UP = i2cReadAck();
	UP <<= 16;
	UP |= (i2cReadAck() << 8);
	UP |= i2cReadNack();
	UP >>= (8-oversampling);
	return UP;
}

// Reads the true pressure value in Pa
int32_t readPressure(void) {
	int32_t UT, UP, B3, B6, X1, X2, X3, p;
	uint32_t B4, B7;
	
	UT = readRawTemperature();
	UP = readRawPressure();
	X1 = ((UT - (int32_t)ac6) * ((int32_t)ac5) >> 15);
	X2 = (((int32_t)mc << 11) / (X1 + (int32_t)md));
	B6 = (X1 + X2) - 4000;
	X1 = ((int32_t)b2 * ((B6 * B6) >> 12 )) >> 11;
	X2 = ((int32_t)ac2 * B6) >> 11;
	X3 = X1 + X2;
	B3 = ((((int32_t)ac1 * 4 + X3) << oversampling) + 2) / 4;
	X1 = ((int32_t) ac3 * B6) >> 13;
	X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
	B7 = ((uint32_t)UP - B3) * (uint32_t)(50000UL >> oversampling);
	if (B7 < 0x80000000) {
		p = (B7 * 2)/B4;
	} else {
		p = (B7/B4) * 2;
	}
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + (int32_t)3791) >> 4);
	return p;
}

// Estimates the Absolute Altitude in meters, based on the pressure value
float readAbsAltitude(void) {
	float p = readPressure();
	return (44330 * (1.0 - pow((p/101325), 0.1903)));
}

#endif /* BMP085_H_ */