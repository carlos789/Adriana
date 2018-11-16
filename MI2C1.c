#include <p33EP512GM304.h>//#include <p33fj128gp202.h>
#include "MI2C1.h"

void OpenI2C1(void){
	I2C1CON = 0x8200;
	I2C1BRG = 0x5D;//400 kHz con Fcy=40Mhz o 0x02D;//para 400 kHz con Fcy=20Mhz;
        //  #define FSCL_I2C 400000	// VALOR DE BAUDE RATE DE COMUNICACIÓN I2C
        //#define I2CBRGVAL ((FCY/FSCL_I2C)-(FCY/1111111))-1


}

void StartI2C1(void){
	I2C1CONbits.SEN = 1;
	while(I2C1CONbits.SEN);
}

void RestartI2C1(void){
	I2C1CONbits.RSEN = 1;
	while(I2C1CONbits.RSEN);
}

void StopI2C1(void){
	I2C1CONbits.PEN = 1;
	while(I2C1CONbits.PEN);
}

void AckI2C1(void){
	I2C1CONbits.ACKDT=0;
	I2C1CONbits.ACKEN=1;
	while(I2C1CONbits.ACKEN);
}

void WaitAckI2C1(void){
	while((I2C1STATbits.TRSTAT == 1)||(I2C1STATbits.ACKSTAT == 1));
}

void NotAckI2C1(void){
	I2C1CONbits.ACKDT=1;
	I2C1CONbits.ACKEN=1;
	while(I2C1CONbits.ACKEN);
}

unsigned char DataRdyI2C1(void){
	if(I2C1STATbits.RBF)   
    	return(+1);
  	else
    	return(0);
}

void WriteI2C1(unsigned char data_out){
	I2C1TRN = data_out;
}

void OpenI2C2(void){
	I2C2CON = 0x8200;
	I2C2BRG = 0x5D;//400 kHz con Fcy=40Mhz o 0x02D;//para 400 kHz con Fcy=20Mhz;
        //  #define FSCL_I2C 400000	// VALOR DE BAUDE RATE DE COMUNICACIÓN I2C
        //#define I2CBRGVAL ((FCY/FSCL_I2C)-(FCY/1111111))-1


}

void StartI2C2(void){
	I2C2CONbits.SEN = 1;
	while(I2C2CONbits.SEN);
}

void RestartI2C2(void){
	I2C2CONbits.RSEN = 1;
	while(I2C2CONbits.RSEN);
}

void StopI2C2(void){
	I2C2CONbits.PEN = 1;
	while(I2C2CONbits.PEN);
}

void AckI2C2(void){
	I2C2CONbits.ACKDT=0;
	I2C2CONbits.ACKEN=1;
	while(I2C2CONbits.ACKEN);
}

void WaitAckI2C2(void){
	while((I2C2STATbits.TRSTAT == 1)||(I2C2STATbits.ACKSTAT == 1));
}

void NotAckI2C2(void){
	I2C2CONbits.ACKDT=1;
	I2C2CONbits.ACKEN=1;
	while(I2C2CONbits.ACKEN);
}

unsigned char DataRdyI2C2(void){
	if(I2C2STATbits.RBF)
    	return(+1);
  	else
    	return(0);
}

void WriteI2C2(unsigned char data_out){
	I2C2TRN = data_out;
}
