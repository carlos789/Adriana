#include <p33EP512GM304.h>
#define FCY 60000000UL//
#include <libpic30.h>
#include "RF.h"
#include "IMU.h"
#include "MSPI1.h"
#include "UARTSD.h"

extern char cachito[32];
extern char recepcion[32];
extern char VectorRS[32];

extern char PIPE_0[5];
extern char PIPE_1[5];
extern char PIPE_2[5];
extern char PIPE_3[5];
extern char PIPE_4[5];
extern char PIPE_5[5];

extern char N_pipe;

extern char error_MAX_RT;

char aux1;
//**********Funciones para SPI1****************************

void RFREAD(char comando, char *SPI_IN, unsigned char cantidad) {
    CS_RF = 0; //bajar chip select
    unsigned char i;
    WByteSPI3(comando);

    for (i = 0; i < cantidad; i++) {
        SPI_IN[i] = RByteSPI3();
        Nop();
    }
    CS_RF = 1; //Subir chip selecto
} //ORIGINAL

unsigned char RFBREAD(char reg)//Lectura de un Byte
{
    unsigned char value;
    CS_RF = 0;
    __delay_us(1); // A modo de prueba  
    WByteSPI3(reg);
    __delay_us(1); // A modo de prueba    
    value = RByteSPI3();
    Nop();
    __delay_us(1); // A modo de prueba 
    CS_RF = 1;
    return (value);
}

void RFWRITE(char registro, char *SPI_OUT, unsigned char cantidad) {
    unsigned char i;
    CS_RF = 0; //bajar chip select 
    __delay_us(1); // A modo de prueba  
    WByteSPI3(registro);
    __delay_us(1); // A modo de prueba  
    for (i = 0; i < cantidad; i++) {
        WByteSPI3(SPI_OUT[i]);
        Nop();
    }
    __delay_us(1); // A modo de prueba     
    CS_RF = 1; //subir chip select
} //original

void RFBWRITE(char registro, char valor) {
    CS_RF = 0; //bajar chip select 
    __delay_us(1); // A modo de prueba  
    WByteSPI3(registro);
    __delay_us(1); // A modo de prueba  
    WByteSPI3(valor);
    __delay_us(1); // A modo de prueba     
    CS_RF = 1; //subir chip select
}

void RF_TX(char data_RF) {
    //IEC1bits.INT1IE=0;
    RFBWRITE(W_CONFIG, 0b01011010); //PWR_UP=1, PRIM_RX=0, MASK_TX=0, MASK_RX=0
    cambiar_pipe(3);
    RFBWRITE(W_TX_PAYLOAD, data_RF); //cargamos el dato a transmitir
    CE_RF = 1;
    //while ((RFBREAD(R_STATUS)&0b00100000)==0);
    while (IRQ_RF == 1);
    //esperamos a que termine la transmision
    RFBWRITE(W_STATUS, 0b01110000);
    CE_RF = 0;
    //IEC1bits.INT1IE=1;
}

char RF_RX() {
    char recep;
    RFBWRITE(W_EN_RXADDR, 0b00000001);
    RFBWRITE(W_CONFIG, 0b00111011); //PWR_UP=1, PRIM_RX=1,MASK_TX=0, MASK_RX=0

    RFBWRITE(W_EN_RXADDR, 0b00111111); //habilitacion de todos los pipes
    RFWRITE(W_RX_ADDR_P0, PIPE_0, 5); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P1, PIPE_1, 5); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P2, PIPE_2, 1); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P3, PIPE_3, 1); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P4, PIPE_4, 1); //direccion del pipe de recepcion
    RFWRITE(W_RX_ADDR_P5, PIPE_5, 1); //direccion del pipe de recepcion
    RFBWRITE(W_RX_PW_P5, 0b00000001); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P4, 0b00000001); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P3, 0b00000001); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P2, 0b00000001); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P1, 0b00000001); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P0, 0b00000001); //fijando el largo del dato 

    CE_RF = 1;
    //while ((RFBREAD(R_STATUS)&0b01000000)==0);//esperamos a que termine la transmision
    while (IRQ_RF == 1);
    CE_RF = 0;
    //    RFREAD(R_RX_PAYLOAD);
    recep = RFBREAD(R_RX_PAYLOAD);
    RFBWRITE(W_STATUS, 0b01110000);
    return (recep);
}

char RF_RX_32(char *payloadRX) {
    char pipe;
    IEC1bits.INT1IE = 0;
    RFBWRITE(W_CONFIG, 0b00111011); //PWR_UP=1, PRIM_RX=1,MASK_TX=1, MASK_RX=0,MASK_MAX_RT=1,
    __delay_us(150); //esperar el timepo Tpd2stby
    RFBWRITE(W_EN_RXADDR, 0b00111111); //habilitacion de todos los pipes
    RFWRITE(W_RX_ADDR_P0, PIPE_0, 5); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P1, PIPE_1, 5); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P2, PIPE_2, 1); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P3, PIPE_3, 1); //direccion del pipe de recepcion 
    RFWRITE(W_RX_ADDR_P4, PIPE_4, 1); //direccion del pipe de recepcion
    RFWRITE(W_RX_ADDR_P5, PIPE_5, 1); //direccion del pipe de recepcion
    RFBWRITE(W_RX_PW_P5, 0b00100000); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P4, 0b00100000); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P3, 0b00100000); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P2, 0b00100000); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P1, 0b00100000); //fijando el largo del dato
    RFBWRITE(W_RX_PW_P0, 0b00100000); //fijando el largo del dato  
    CE_RF = 1;
    while (IRQ_RF == 1);
    CE_RF = 0;
    pipe = ((RFBREAD(R_STATUS))& 0b00001110) >> 1; //muestra el numero de pipe por el que recibe
    RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
    RFREAD(R_RX_PAYLOAD, payloadRX, 32);
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
    return pipe;
} //ORIGINAL

char RF_TX_32_NOACK(char * payloadTX, char pipe) {
    IEC1bits.INT1IE = 0;
    CE_RF = 0;
    RFBWRITE(W_CONFIG, 0b01001010); //PWR_UP=1, PRIM_RX=0, MASK_TX=0, MASK_MAX_RT=0
    __delay_us(150); //esperar el timepo Tpd2stby
   
    RFBWRITE(W_STATUS, 0b01111110); //bajamos las baderas internas del modulo
    RFBWRITE(W_EN_AA, 0b00111111);
    RFBWRITE(W_EN_RXADDR, 0b00111111);
    
    RFBWRITE(W_FEATURE, 0x01); //habilitamos la transmision sin ack 
    cambiar_pipe(pipe);
    RFWRITE(W_TX_PAYLOAD_NO_ACK, payloadTX, 32); //cargamos el dato a transmitir
    CE_RF = 1;
    __delay_us(10);
    CE_RF = 0;
    RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
    //RFBWRITE(W_CONFIG, 0b01111000); //enmascaramos las interrupciones del modulo
     RFBWRITE(W_FEATURE, 0x00); //habilitamos la transmision sin ack 
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
}

char RF_TX_32(char * payloadTX, char pipe) {
    IEC1bits.INT1IE = 0;
    //RFBWRITE(FLUSH_TX,0);
    CE_RF = 0;
    error_MAX_RT = 0;
    RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
    RFBWRITE(W_EN_AA, 0b00111111);
    RFBWRITE(W_CONFIG, 0b01001010); //PWR_UP=1, PRIM_RX=0, MASK_TX=0, MASK_MAX_RT=0
    __delay_us(150); //esperar el timepo Tpd2stby
    cambiar_pipe(pipe);
    RFWRITE(W_TX_PAYLOAD, payloadTX, 32); //cargamos el dato a transmitir
    CE_RF = 1;
    while (IRQ_RF == 1) {
        //U1TXREG = 0xcc;
    }; //esperamos a que termine la transmision
    CE_RF = 0;
    if (((RFBREAD(R_STATUS))&0b00010000) == 0b00010000) {//nos fijamos si no se pudo concretar la transmision y se sobrepaso la maxima cantidad de intentos
        while (!U1STAbits.TRMT);
        U1TXREG = 0xaa; //todomal no se c+pudo comunicar, que hacemos ahi???
        error_MAX_RT = 1;
        //RFWRITE(FLUSH_TX,0); 
    }
    RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
    RFBWRITE(W_CONFIG, 0b01111000); //enmascaramos las interrupciones del modulo
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
    return 0;
} //ORIGINAL

char RF_TX_plus(char * payloadTX, char pipe, int cantidad_datos) {
    IEC1bits.INT1IE = 0;
    char l = 0;
    int cantidad = 0;
    CE_RF = 0;
    error_MAX_RT = 0;
    RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
    RFBWRITE(W_EN_AA, 0b00111111);
    RFBWRITE(W_CONFIG, 0b01001010); //PWR_UP=1, PRIM_RX=0, MASK_TX=0, MASK_MAX_RT=0
    __delay_us(150); //esperar el timepo Tpd2stby
    cambiar_pipe(pipe);
    while (cantidad < cantidad_datos) {
        RFWRITE(W_TX_PAYLOAD, payloadTX + l * 32, 32); //cargamos el dato a transmitir
        CE_RF = 1;
        while (IRQ_RF == 1); //esperamos a que termine la transmision
        CE_RF = 0;
        //agregar if de max_RT
        l++;
        cantidad = cantidad + 32;
        RFBWRITE(W_STATUS, 0b01110000);
    }
    if (((RFBREAD(R_STATUS))&0b00010000) == 0b00010000) {//nos fijamos si no se pudo concretar la transmision y se sobrepaso la maxima cantidad de intentos
        U1TXREG = 0xaa; //todomal no se c+pudo comunicar, que hacemos ahi???
        error_MAX_RT = 1;
    }
    RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
    RFBWRITE(W_CONFIG, 0b01111000); //enmascaramos las interrupciones del modulo
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
    return 0;
} //modificacion para muchos datos (mas de 32)

char cambiar_pipe(char pipe) {
    switch (pipe) {
        case 0:
            RFWRITE(W_TX_ADDR, PIPE_0, 5); // darle la direccion al transmisor               
            RFWRITE(W_RX_ADDR_P0, PIPE_0, 5); // darle la direccion del pipe 0 para ACK
            break;
        case 1:
            RFWRITE(W_TX_ADDR, PIPE_1, 5); // darle la direccion al transmisor
            RFWRITE(W_RX_ADDR_P0, PIPE_1, 5); // darle la direccion del pipe 0 para ACK
            break;
        case 2:
            RFWRITE(W_TX_ADDR, PIPE_2, 5); // darle la direccion al transmisor
            RFWRITE(W_RX_ADDR_P0, PIPE_2, 5); // darle la direccion del pipe 0 para ACK
            break;
        case 3:
            RFWRITE(W_TX_ADDR, PIPE_3, 5); // darle la direccion al transmisor
            RFWRITE(W_RX_ADDR_P0, PIPE_3, 5); // darle la direccion del pipe 0 para ACK
            break;
        case 4:
            RFWRITE(W_TX_ADDR, PIPE_4, 5); // darle la direccion al transmisor
            RFWRITE(W_RX_ADDR_P0, PIPE_4, 5); // darle la direccion del pipe 0 para ACK
            break;
        case 5:
            RFWRITE(W_TX_ADDR, PIPE_5, 5); // darle la direccion al transmisor
            RFWRITE(W_RX_ADDR_P0, PIPE_5, 5); // darle la direccion del pipe 0 para ACK
            break;

    }
}

char conf_recepcion_slave() {
    IEC1bits.INT1IE = 0;
    char m;
    for (m = 0; m < 32; ++m) {
        VectorRS[m] = 0;
    }
    RFBWRITE(W_CONFIG, 0b00111011); //PWR_UP=1, PRIM_RX=1,MASK_TX=1, MASK_RX=0,MASK_MAX_RT=1,
    __delay_us(150); //esperar el timepo Tpd2stby  
    aux1 = ((0b00000001 << N_pipe) | 0b00000001);
    //aux1=0b00000001;
    RFBWRITE(W_EN_RXADDR, aux1); //habilitacion de todos los pipes AQUI SE VARIA POR QUE PIPE SE COMUNICA EL SLAVE
    RFWRITE(W_RX_ADDR_P0, PIPE_0, 5); //reestablecer direccion del pipe0 de recepcion
    RFBWRITE(W_EN_AA, 0B00111111); //ACTIVAR AUTO ACNOLESDKIVBDSF
    CE_RF = 1;
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
}