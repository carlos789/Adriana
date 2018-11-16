/*
 *      Código para experiencia con MMA8451
 * Se agrega interrupciones por INT0  , corresponde al pin RB7
 * FCY=60MIPS
 *Fecha:12/11/15
 */
#include <p33EP512GP502.h>
#include <stdio.h>
#define FCY 60000000UL//
#include <libpic30.h>
#include <uart.h>
#include <math.h>
_FICD(ICS_PGD3 & JTAGEN_OFF);// Para hacer debuging por el puerto 2
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
_FPOR(ALTI2C2_ON) ;//  se usa I2C alternativo pines

/* ** ********************Funciones****************************************************** ** */

/**************************************Registros*******************************************/

/***********************Fin de Registros***************************************************/

int main (void){
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CONDICIONES INICIALES y DECLARACIONES DE VARIABLES ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//****************
   Config1();// Configuración de USART y mapeo de pines
   /*********************Interrupción***************************************************/
           
   IntInit();//Rutina de inicialización de interrupciones
   while(1)
   {
   }
}

//********************************************************************************************

 /********************************************************************************/
void Config1(void)// Configuraciób de puertos y otros
{
    #define FCY 60000000UL
    #define BAUDRATE 19200
    #define BRGVAL ((FCY/BAUDRATE)/4) - 1//con
    // Configure Oscillator to operate the device at 60 MHz
    // Fosc = Fin * M/(N1 * N2), Fcy = Fosc/2
    // Fosc = 8M * 60/(2 * 2) = 60 MHz for 8M input clock

        PLLFBD = 58;//78; 			// M = 60
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
	U1STAbits.UTXINV = 0;		// UxTX Idle state is ?1?
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
  INTCON2 = 0x0000;   /*Setup INT0,  interupt on falling edge*/
   IFS0bits.INT0IF = 0;    /*Reset INT0 interrupt flag */
   IPC0=0x07;// Alta prioridad para INT0
   IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */
   TRISBbits.TRISB7=1;// INT0 entrada
}

void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{
   
   IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag or else
   printf("s");// A nmodo de ejemplo
           }


   void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt( void )
 {
       int dato;
 
 dato=U1RXREG;
 IFS0bits.U1RXIF = 0; // Clear RX Interrupt flag
// printf("Interrupcion\n");
// printf("Llego el dato: %c\n", dato);
   
 }      
       