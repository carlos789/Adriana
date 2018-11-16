/*
 *      Código para experiencia con FAS1500 
 * Se agrega interrupciones por INT0  , corresponde al pin RB7
 * FCY=20MIPS
 *Fecha:21/06/13
 */
#include <p33EP512GP502.h>
#include <stdio.h>
#include "MI2C1.h"
#include "IMU.h"
#include "UARTSD.h"
#include <stdio.h> /* para usar  printf */
#define FCY 40000000UL//
#include <libpic30.h>
#include <uart.h>
#include <math.h>
_FICD(ICS_PGD3 & JTAGEN_OFF);// Para hacer debuging por el puerto 2
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
_FPOR(ALTI2C2_ON) ;//  se usa I2C alternativo pines

/* ** ********************Funciones****************************************************** ** */
void Config1(void);// Configuraciób de puertos y otros
void IntInit(void);
void EnviarTX(void);
void EnviarMMA(void);
void MMA845XActivo(void);

 void readotro(char cod,char reg1, int count);
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void);// Rutina de interrupcion INT0
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt( void );//Rutina de interrupcion
/* ****************************************************************************************/


#define LED1	PORTBbits.RB0



/* ** *********************************************************************************** ** */

char Vector[6];//Vector para almacenar datos de Temp,Gyro y Aceleración
char VectorRS[55];//Por ahora son 55 bytes
char VectorMMA[192];//
float VOffset[3];
int j;
int Cuenta=0;// Registro de numero de vectores leidos
char cmdW = 0x38;// codigo escritura
char cmdR = 0x39;// Código de lectura
int OUT_X = 0;
int OUT_Y = 0;
int OUT_Z = 0;
unsigned char Vcuenta = 0;
float temp = 0.0;
unsigned char orden,Kbhit,parar;
int Dato;//Variable de lectura del acelerómetro
char Dire, Valor,CTRL_REG1,INT_SOURCE;
/***********************Registro de Jaime**********************************/


int main (void){

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CONDICIONES INICIALES y DECLARACIONES DE VARIABLES ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//****************
   Config1();// Configuración de USART y mapeo de pines
  // printf("Inicio Placa DSPIC\n");
   
   
   OpenI2C2();//Configuro I2C

/***********************************************************************
 Configuracion de la IMU
  ************************************************************************/

IMUWRITE2(cmdW,0x2A,0x1);// MMA se pone en modo activo
//__delay_ms(10); // tiempo necesario antes de iniciar otra comunicacion, en el caso de escritura
//
//IMUWRITE(cmdW,0x14,0x53);// Registro 20 configuracion del CLKOUTEN, habilita el reloj de salida
//__delay_ms(10); // tiempo necesario antes de iniciar otra comunicacion, en el caso de escritura
//
//
//LED1=0;
//Dato=IMUBREAD(cmdW,0x00);//Leo registro 0
//printf("Quien soy yo: %c\n", Dato);
//Dato=IMUBREAD(0xA6,0x00);//Leo registro 0x2D
//IMUWRITE(cmdW,0x3D,0x28);//// Set passthrough mode to Accel so we can turn it on
//__delay_ms(10);

 
/*********************Interrpción***************************************************/
           //Rutina de inicialización de interrupciones
        IntInit();
/* ***************I2C Inicialización************************************************/
//        Dato=IMUBREAD(cmdW,0x1A);//Leo registro 26 Estado de Interrupciones
        /* de paso bajo la bandera de dato listo en la IMU*/
        //printf("Espero 23: %c\n", Dato);
      Kbhit=0;// Limpio bandera RS
      parar=0;
      while(1)// codigo de procesamiento
            {
             if(Kbhit==1)
          {
                 Kbhit=0;
        switch(orden)
        {
            case 0x41:                //case LEER_REGISTROS:
        VectorRS[0]=0xFE;
        VectorRS[1]=0x35;// 50 valores
        VectorRS[2]= orden;
        for(j=0;j<0x32;j++)
        {
        VectorRS[j+3]=IMUBREAD2(cmdW,j);// Leo todos los registros
        }
        EnviarTX();
        break;

        case 0x42:                //case LEER_REGISTROS:
        VectorRS[0]=0xFE;
        VectorRS[1]=0x09;// 9 valores
        VectorRS[2]= orden;
        for(j=1;j<7;j++)
        {
        VectorRS[j+2]=IMUBREAD2(cmdW,j);//
        }
        EnviarTX();
        break;
        
        case 0x43:                //escribir_REGISTROS:
        MMA845XDesActivo();
        __delay_ms(10);
        IMUWRITE2(cmdW,Dire,Valor);
        VectorRS[0]=0xFE;
        VectorRS[1]=0x03;// 3 valores
        VectorRS[2]= orden;
        EnviarTX();
        MMA845XActivo();
        break;

        case 0x44:                //Leer aceleraciones en forma continua
            parar=1;
            MMA845XDesActivo();
        IMUWRITE2(cmdW,0x09,0x80);//Fifo circular y 32 muestras
        IMUWRITE2(cmdW,0x2D,0x40);//en CTRL_REG4 pongo INT_EN_FIFO=1
//        IMUWRITE2(cmdW,0x2E,0x00);//  en CTRL_REG5 pongo Interrupt is routed to INT2 pin;
//        IMUWRITE2(cmdW,0x2F,0x00);//  en CTRL_REG3 pongo interrupcion en bajo y push-pull
        MMA845XActivo();
        VectorRS[0]=0xFE;
        VectorRS[1]=0xc9;// 201 valores
        VectorRS[2]= orden;
        
        while(parar)
        {

        INT_SOURCE=IMUBREAD2(cmdW,0x0C);//leo INT_SOURCE
       WriteUART1(INT_SOURCE);
	while(BusyUART1());

        if ((INT_SOURCE&0x40) == 0x40)// pregunto si hay interrupcion por FIFO
        {
         IMUBREAD2(cmdW,0x00);// Leo registro F Status
      //  IMUREAD2(cmdW,0x01, 192, VectorMMA);// Leo los 192 valores
        
        EnviarMMA();
        }}
        break;
case 0x45:                //Leer aceleraciones en forma continua
            parar=2;
            MMA845XDesActivo();
        IMUWRITE2(cmdW,0x09,0x00);//Fifo des habilitada
        IMUWRITE2(cmdW,0x2D,0x01);//en CTRL_REG4 pongo INT_EN_DRDY=1
//        IMUWRITE2(cmdW,0x2E,0x00);//  en CTRL_REG5 pongo Interrupt is routed to INT2 pin;
//        IMUWRITE2(cmdW,0x2F,0x00);//  en CTRL_REG3 pongo interrupcion en bajo y push-pull
        MMA845XActivo();
        VectorRS[0]=0xFE;
        VectorRS[1]=21;// 21 valores
        VectorRS[2]= orden;
        Vcuenta=0;
        while(parar==2)
        {
        INT_SOURCE=IMUBREAD2(cmdW,0x0C);//leo INT_SOURCE

        if ((INT_SOURCE&0x01) == 0x01)// pregunto si hay interrupcion por INT_EN_DRDY
        {
            
   for(j=1;j<7;j++)
        {
        VectorRS[j+2+Vcuenta*6]=IMUBREAD2(cmdW,j);//
        }
          Vcuenta=Vcuenta+1;
   if (Vcuenta==3)
   {
        EnviarTX();
        Vcuenta=0;
   } 
   }}
        
        break;
        case 0x46:                //parar de leer
        MMA845XDesActivo();
        IMUWRITE2(cmdW,0x09,0x00);//des habilito la Fifo
        MMA845XActivo();
        VectorRS[0]=0xFE;
        VectorRS[1]=0x3;// 201 valores
        VectorRS[2]= orden;

        EnviarTX();
        break;

default:
          Nop();
          break;
         
        }
               
//               Dato=IMUBREAD2(cmdW,0x0d);// Leo "Who am I" = 0x1A
//                printf("ID: %c\n", Dato);
//                __delay_ms(10);
////               printf("Aceleraciones\n");
               // GET_OUT_XYZ2(&OUT_X, &OUT_Y, &OUT_Z);//GET_OUT_XYZ
               
                //IMUREAD2(cmdW,0x01, 0x06,  *Vector);
//                printf("Acel X: %c\n", OUT_X);
//                printf("Acel Y: %c\n", OUT_Y);
//                printf("Acel Z: %c\n", OUT_Z);
                
//                for(j=0;j<6;j++)
//                {
//                 Dato=IMUBREAD2(cmdW,j+1);
//                U1TXREG= Dato;
//                while(BusyUART1());
//                }
                

      //  __builtin_disi(0x0000); /* enable interrupts */

        }
             LED1=1;
             __delay_ms(500);
             LED1=0;
             __delay_ms(500);
           }
}




/********************************************************************************/
void Config1(void)// Configuraciób de puertos y otros
{
    #define FCY 40000000UL
    #define BAUDRATE 19200
    #define BRGVAL ((FCY/BAUDRATE)/4) - 1//con
    // Configure Oscillator to operate the device at 40 MHz
    // Fosc = Fin * M/(N1 * N2), Fcy = Fosc/2
    // Fosc = 8M * 40/(2 * 2) = 80 MHz for 8M input clock

        PLLFBD = 38;//78; 			// M = 80
	CLKDIVbits.PLLPOST = 0;		// N2 = 2
	CLKDIVbits.PLLPRE = 0; 		// N1 = 2
    OSCTUN = 0;                                 // Tune FRC oscillator, if FRC is used
    RCONbits.SWDTEN = 0;                        /* Disable Watch Dog Timer*/

	__builtin_write_OSCCONH(0x03);	// Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
	__builtin_write_OSCCONL(0x01);
	while(OSCCONbits.COSC != 0b011);// Wait for Clock switch to occur
	while(OSCCONbits.LOCK != 1){};	// Wait for PLL to lock

        ANSELA=0x0000;// Sin entradas analógicas
        ANSELB=0x0000;
//	PORTB = 0;
        
	TRISBbits.TRISB0=0;		// LED_1
	TRISBbits.TRISB8 = 1;		// RX
	TRISBbits.TRISB7 = 0;		//TX
	
//	
        
        OSCCON = 0x46;			// Command Sequence
	OSCCON = 0x57;
        OSCCONbits.IOLOCK = 0;		// Peripherial pin select is not locked

        RPOR2bits.RP39R=0b00001;// UART1_TX -> RP39 Pin16
        RPINR18bits.U1RXR = 0b0101000;	// UART_RX -> RP40 Pin17

        
        
	OSCCON = 0x46;								// Command Sequence
	OSCCON = 0x57;
	OSCCONbits.IOLOCK = 1;		// Peripherial pin select is locked


	// UART CONFIGURATION

	U1MODEbits.PDSEL = 0b00;	// 8-bit data, no parity
	U1MODEbits.STSEL = 0;		// One Stop bit
	U1MODEbits.ABAUD = 0;		// Auto-Baud DISABLED

	U1MODEbits.BRGH = 1;		// Low-speed mode
	U1BRG = BRGVAL;//   Valor inicial para XTPLL=86;									// Baud Rate - 115200 bps

	U1MODEbits.USIDL = 0;		// Continue module operation in Idle mode
	U1MODEbits.IREN = 0;		// IrDA® encoder and decoder disabled
	U1MODEbits.WAKE = 0;		// No wake-up enabled
	U1MODEbits.LPBACK = 0;		// Loopback mode is disabled
	U1MODEbits.URXINV = 0;		// U1RX Idle state is '1'
	U1STAbits.UTXINV = 0;		// UxTX Idle state is ‘1’
	U1STAbits.OERR = 0;		// Clear Overrun Error
	U1STAbits.ADDEN = 0;		// Address Detect mode disabled
	U1STAbits.UTXBRK = 0;		// Sync Break transmission disabled or completed

	U1STAbits.UTXISEL0 = 0;
	U1STAbits.UTXISEL1 = 1;
	IFS0bits.U1TXIF = 0;
	IEC0bits.U1TXIE = 0;
	U1STAbits.URXISEL = 0;
	IFS0bits.U1RXIF = 0;// Limpio la bandera de la interrupción
	IEC0bits.U1RXIE = 1;//Habilito la interrupción por recepción RX

	U1MODEbits.UEN = 0;		// UxTX and UxRX pins are enabled and used;
					// UxCTS and UxRTS/BCLK pins controlled by port latches
	U1MODEbits.UARTEN = 1;		// UART1 is enabled
	U1STAbits.UTXEN = 1;		// Transmit Enabled

	// UART CONFIGURATION



//	AD1CON1bits.ADON = 0;
//
//	AD1CON3bits.ADRC = 0;		// Clock derived from system clock
//	AD1CON3bits.ADCS = 6;		// TCY · (ADCS<7:0> + 1) = 7 · TCY = TAD
//
//	AD1CON1bits.ADSIDL = 0;		// Continue module operation in Idle mode
//	AD1CON1bits.AD12B = 1;		// 12-bit, 1-channel ADC operation
//	AD1CON1bits.FORM = 0b00;	// Integer
//	AD1CON1bits.SSRC = 0b000; 	// Clearing sample bit ends sampling and starts conversion
//	AD1CON1bits.ASAM = 0;		// Sampling begins when SAMP bit is set
//
//	AD1CON2bits.VCFG = 0b000;	// ADREF+ = AVdd, ADREF- = AVss
//	AD1CON2bits.CSCNA = 0;		// Do not scan inputs
//	AD1CON2bits.BUFM = 0;		// Always starts filling buffer at address 0x0
//	AD1CON2bits.ALTS = 0;		// Always uses channel input selects for Sample A
//
//	AD1CHS0bits.CH0NA = 0;		// Channel 0 negative input is VREF
//	AD1CHS0bits.CH0SA = 0b00001;	// Channel 0 positive input is AN1
//
//	AD1CON1bits.ADON = 1;
}

/* ****************Interrupciones*********************************************/
void IntInit(void)
{
//   INTCON2 = 0x0000;   /*Setup INT0,  interupt on falling edge*/
//   IFS0bits.INT0IF = 0;    /*Reset INT0 interrupt flag */
//   IPC0=0x07;// Alta prioridad para INT0
//   IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */
//
//   TRISBbits.TRISB7=1;// INT0 entrada
}

void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{
   
   IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag or else
printf("s");
   
        }


   void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt( void )
 {
       int dato;
 
 dato=U1RXREG;
 if (dato==0xFD)
 {
 while (!DataRdyUART1());
 dato=U1RXREG;
  while (!DataRdyUART1());
 orden=U1RXREG;
 if (orden==0x43)// llegan 5 bytes
 {
     while (!DataRdyUART1());
 Dire=U1RXREG;
  while (!DataRdyUART1());
 Valor=U1RXREG;
 }
 if(orden==0x46)
 {parar=0;
 }
 Kbhit=1;
 }
 IFS0bits.U1RXIF = 0; // Clear RX Interrupt flag
// printf("Interrupcion\n");
// printf("Llego el dato: %c\n", dato);
   
        }

   void EnviarTX(void)
   {
       unsigned int i;
       for (i=0;i<VectorRS[1];i++)
       {
           while(BusyUART1());
       U1TXREG= VectorRS[i];
       
       }

   }

 void MMA845XActivo(void)
   {
     CTRL_REG1=IMUBREAD2(cmdW,0x2A);//Leo el registro 0x2A
     CTRL_REG1=CTRL_REG1 | 0x01;//seteo el bit ACTIVE
     IMUWRITE2(cmdW,0x2A,CTRL_REG1);// MMA se pone en modo activo
 }
 void MMA845XDesActivo(void)
 {
     CTRL_REG1=IMUBREAD2(cmdW,0x2A);//Leo el registro 0x2A
     CTRL_REG1=CTRL_REG1 & 0xFE;//Limpio el bit ACTIVE
     IMUWRITE2(cmdW,0x2A,CTRL_REG1);// MMA se pone en modo desactivo
 }

 void EnviarMMA(void)
   {
       unsigned int i;
       for (i=0;i<3;i++)
       {
       U1TXREG= VectorRS[i];
       while(BusyUART1());
       }
 for (i=0;i<192;i++)
       {
       U1TXREG= VectorMMA[i];
       while(BusyUART1());
       }

 }
 
