#include <p33EP512GM304.h>//#include <p33fj128gp202.h>
#include "MSPI1.h"

// ********************SPI1 para acelerómetro **********************************
void OpenSPI1(void){// Para el SPI1
IEC0bits.SPI1IE = 0; //Desactivation de la interrupcion SPI
SPI1STAT = 0x0; //Desactivation del modulo
SPI1CON1=0x0;
SPI1CON2=0;// No Buffer enhanced
SPI1CON1bits.DISSCK = 0; //Activation Clock
SPI1CON1bits.DISSDO = 0; //Activation SDO
SPI1CON1bits.MODE16 = 0; //Communication modo byte.
SPI1CON1bits.SSEN=0; //Activation del Slave
SPI1CON1bits.CKE = 1;
SPI1CON1bits.CKP = 0;
SPI1CON1bits.MSTEN = 1; //Master Mode
SPI1CON1bits.SMP = 0;
SPI1CON1bits.SPRE=0b101; //0b001; //Secondaire prescaler 1:1
SPI1CON1bits.PPRE=0b00; //0b10; //Primario prescaler 64:1 -->Clock 0,93 MHZ
SPI1STATbits.SPIEN = 1; //Activation del module SPI
//SPI2STATbits.SISEL = 1; //Activation interrupcion bufer RX
IFS0bits.SPI1IF=0;
IEC0bits.SPI1IE=0;

}
unsigned char RByteSPI1(void)
    {
    int temp;
    temp = SPI1BUF;        // dummy read of the SPI1BUF register to clear the SPIRBF flag
    IFS0bits.SPI1IF=0;
    SPI1BUF = 0x00;        // write the data out to the SPI peripheral
    Nop();
   // while( !IFS0bits.SPI1IF );
   while( !SPI1STATbits.SPIRBF );//
    // while( SPI1STATbits.SPITBF );//
    return(SPI1BUF);
  	}

void WByteSPI1(unsigned char data_out)
    {
	int temp;
    temp = SPI1BUF;                             // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI1BUF = data_out & 0xFF;                   // write the data out to the SPI peripheral
    while( !SPI1STATbits.SPIRBF );              // wait for the data to be sent out
    }
// ********************SPI2 para memoria serial **********************************
void OpenSPI2(void){ //Para el SPI2
IEC2bits.SPI2IE = 0; //Desactivation de la interrupcion SPI
SPI2STAT = 0x0; //Desactivation del modulo
SPI2CON1=0x0;
SPI2CON2=0;// No Buffer enhanced
SPI2CON1bits.DISSCK = 0; //Activation Clock
SPI2CON1bits.DISSDO = 0; //Activation SDO
SPI2CON1bits.MODE16 = 0; //Communication byte.
SPI2CON1bits.SSEN=0; //Activation del Slave 
SPI2CON1bits.CKE = 1;//
SPI2CON1bits.CKP = 0; //
SPI2CON1bits.MSTEN = 1; //Master Mode
SPI2CON1bits.SMP = 0;
//SPI2CON1bits.SPRE=0b111; //Prescaler secundario 1:1
//SPI2CON1bits.PPRE=0b00; //Primairio prescaler 64:1 -->Clock 0,93 MHZ Tiempo escritura pagina 2,8 ms
SPI2CON1bits.SPRE=0b011; //Prescaler secundario 3:1
SPI2CON1bits.PPRE=0b10; //Primairio prescaler 4:1 -->Clock 5 MHZ Tiempo escritura pagina 1 ms
//Tiempo de lectura de una página=1,04 ms
SPI2STATbits.SPIEN = 1; //Activation del module SPI
//SPI2STATbits.SISEL = 1; //Activation interrupcion bufer RX
IFS2bits.SPI2IF=0;
IEC2bits.SPI2IE=0;
}


unsigned char RByteSPI2(void)
    {
    int temp;
    temp = SPI2BUF;                   // dummy read of the SPI2BUF register to clear the SPIRBF flag
////    IFS2bits.SPI2IF=0;
//    SPI2BUF = 0xF0;                   // write the data out to the SPI peripheral
//    Nop();
//    while( !SPI2STATbits.SPIRBF );     // Espero que llegue el dato
////    while( !IFS2bits.SPI2IF );
//   //  __delay_us(50);
//    return(SPI2BUF);
    IFS2bits.SPI2IF=0;
    SPI2BUF = 0xF0;        // write the data out to the SPI peripheral
    Nop();
    while( !IFS2bits.SPI2IF );
    return(SPI2BUF);
	}

void WByteSPI2(unsigned char data_out)
    {
	int temp;
   // while (SPI2STATbits.SPITBF == 1);
    temp = SPI2BUF;                             // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI2BUF = data_out & 0xFF;                   // write the data out to the SPI peripheral
    while( !SPI2STATbits.SPIRBF );
   // while( !SPI2STATbits.SPIRBF );              // wait for the data to be sent out
    }

// ********************SPI3 para moduloRF **********************************
void OpenSPI3(void){ //Para el SPI3
IEC5bits.SPI3IE = 0; //Desactivation de la interrupcion SPI
SPI3STAT = 0x0; //Desactivation del modulo
SPI3CON1=0x0;
SPI3CON2=0;// No Buffer enhanced
SPI3CON1bits.DISSCK = 0; //Activation Clock
SPI3CON1bits.DISSDO = 0; //Activation SDO
SPI3CON1bits.MODE16 = 0; //Communication byte.
SPI3CON1bits.SSEN=0; //desActivation del Slave 
SPI3CON1bits.CKE = 1;//
SPI3CON1bits.CKP = 0; //
SPI3CON1bits.MSTEN = 1; //Master Mode
SPI3CON1bits.SMP = 1;//CAMBIO! era "0" 2/11/2016 lo vi en una pagina


//SPI3CON1bits.SPRE=0b111; //Prescaler secundario 1:1
//SPI3CON1bits.PPRE=0b00; //Primairio prescaler 64:1 -->Clock 0,93 MHZ Tiempo escritura pagina 2,8 ms
SPI3CON1bits.SPRE=0b101; //0b001; //Secondaire prescaler 1:1
SPI3CON1bits.PPRE=0b00; //0b10; //Primario prescaler 64:1 -->Clock 0,93 MHZ
//SPI3CON1bits.SPRE=0b011; //Prescaler secundario 3:1
//SPI3CON1bits.PPRE=0b10; //Primairio prescaler 4:1 -->Clock 5 MHZ Tiempo escritura pagina 1 ms
//Tiempo de lectura de una página=1,04 ms
SPI3STATbits.SPIEN = 1; //Activation del module SPI
//SPI3STATbits.SISEL = 1; //Activation interrupcion bufer RX
IFS5bits.SPI3IF=0;
IEC5bits.SPI3IE=0;
}


unsigned char RByteSPI3(void)
    {
    SPI3STATbits.SPIROV = 0;
    IFS5bits.SPI3IF = 0;
    IFS5bits.SPI3EIF = 0;
    int temp;
    temp = SPI3BUF;                   // dummy read of the SPI3BUF register to clear the SPIRBF flag
////    IFS2bits.SPI3IF=0;
//    SPI3BUF = 0xF0;                   // write the data out to the SPI peripheral
//    Nop();
//    while( !SPI3STATbits.SPIRBF );     // Espero que llegue el dato
////    while( !IFS2bits.SPI3IF );
//   //  __delay_us(50);
//    return(SPI3BUF);
    IFS5bits.SPI3IF=0;
    SPI3BUF = 0xF0;        // write the data out to the SPI peripheral
    Nop();
    //while(SPI3STATbits.SPIRBF==0);
    while( !IFS5bits.SPI3IF );
    return(SPI3BUF);
	}

void WByteSPI3(unsigned char data_out)
    {
	int temp;
   // while (SPI3STATbits.SPITBF == 1);
    temp = SPI3BUF;                             // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI3BUF = data_out & 0xFF;                   // write the data out to the SPI peripheral
    //while(SPI3STATbits.SPIRBF);
    while( !SPI3STATbits.SPIRBF );              // wait for the data to be sent out
    }



