#ifndef MSPI_H // header guards
#define MSPI_H
#define FCY 60000000UL//
#include <libpic30.h>

#define  CS_FX  LATBbits.LATB4
#define  CS_M   LATAbits.LATA1
#define  CS_SD  LATCbits.LATC0
#define  CS_RF  LATBbits.LATB6
#define  CE_RF  LATCbits.LATC7
#define  IRQ_RF PORTBbits.RB10

#define SPISTAT_RBF1         SPI1STATbits.SPIRBF
#define SPISTAT_TBF1         SPI1STATbits.SPITBF
#define SPISTAT_RBF2         SPI2STATbits.SPIRBF
#define SPISTAT_TBF2         SPI2STATbits.SPITBF
#define SPISTAT_RBF3         SPI3STATbits.SPIRBF
#define SPISTAT_TBF3         SPI3STATbits.SPITBF

void OpenSPI1(void);
unsigned char RByteSPI1(void);
void WByteSPI1(unsigned char data_out);
void OpenSPI2(void);
unsigned char RByteSPI2(void);
void WByteSPI2(unsigned char data_out);
void OpenSPI3(void);
unsigned char RByteSPI3(void);
void WByteSPI3(unsigned char data_out);


union Adress    ////Dirección a escribir de la memoria
    {
        unsigned long int Complete;
        char Singles[4];
    };
 
 union AdressRead    //Direccion a leer de la memoria
    {
        unsigned long int Complete;
        char Singles[4];
    };

#endif
