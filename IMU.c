#include <p33EP512GM304.h>
#include "IMU.h"
#include "MSPI1.h"
#include "UARTSD.h"
#define FCY 60000000UL//
#include <libpic30.h>
extern char cmdW;
extern char VectorMEM[192];//
//extern float WZOffset;

//**********Funciones para SPI1****************************
void IMUREAD(char reg1, int count, unsigned char array[]){
    unsigned int i;
   unsigned  char dire, dire1;
    dire =reg1 &0x7F;
    dire1= reg1 &0x80;
	CS_FX=0;
	WByteSPI1(dire);
	WByteSPI1(dire1);
		
	for(i = 0; i < count; i++){
		array[i] = RByteSPI1();
		}
	CS_FX=1;

}



void IMUWRITE(char reg, char value){
	unsigned  char dire, dire1;
   dire =reg|0x80;
   dire1= reg &0x80;
	CS_FX=0;
	WByteSPI1(dire);
    WByteSPI1(dire1);
    WByteSPI1(value);
	CS_FX=1;
}

unsigned char IMUBREAD(char reg)//Lectura de un Byte
{
unsigned  char dire, dire1, value;
dire =reg &0x7F;
dire1= reg &0x80;
CS_FX=0;
__delay_us(1);// A modo de prueba  
WByteSPI1(dire);
//__delay_us(100);// A modo de prueba  
WByteSPI1(dire1);
//__delay_us(100);// A modo de prueba  
value=RByteSPI1();
 Nop(); 
 __delay_us(1);// A modo de prueba 
CS_FX=1;
return(value);
}

void IMUREAD2(char reg1, int count, unsigned char array[]){
unsigned int i;
unsigned  char dire, dire1;
dire =reg1 &0x7F;
dire1= reg1 &0x80;
CS_FX=0;
WByteSPI1(dire);
WByteSPI1(dire1);
	for(i = 0; i < count; i++){
		
		VectorMEM[i]=RByteSPI1();//
		}
Nop(); 
CS_FX=1;
return;
	}

	


void SET_SCALE(unsigned char scale){
	IMUWRITE(0x0E, scale);
}



void GET_OUT_XYZ(int *OUT_X, int *OUT_Y, int *OUT_Z){

	char OUT_XYZ[6];

	IMUREAD( 0x01, 6, OUT_XYZ);

	*OUT_X = 0;
	*OUT_X = (OUT_XYZ[0] << 4)|((OUT_XYZ[1] >> 4) & 0x000F);

	*OUT_Y = 0;
	*OUT_Y = (OUT_XYZ[2] << 4)|((OUT_XYZ[3] >> 4) & 0x000F);

	*OUT_Z = 0;
	*OUT_Z = (OUT_XYZ[4] << 4)|((OUT_XYZ[5] >> 4) & 0x000F);

}

void GET_OUT_XYZ2(int *OUT_X, int *OUT_Y, int *OUT_Z){

	char OUT_XYZ[6];

	IMUREAD2( 0x01, 6, OUT_XYZ);

	*OUT_X = 0;
	*OUT_X = (OUT_XYZ[0] << 4)|((OUT_XYZ[1] >> 4) & 0x000F);

	*OUT_Y = 0;
	*OUT_Y = (OUT_XYZ[2] << 4)|((OUT_XYZ[3] >> 4) & 0x000F);

	*OUT_Z = 0;
	*OUT_Z = (OUT_XYZ[4] << 4)|((OUT_XYZ[5] >> 4) & 0x000F);

}
