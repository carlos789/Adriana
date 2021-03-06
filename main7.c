/*
 *      C�digo para experiencia con aceler�metro FXLS8471 
 * Se agrega interrupciones por INT0  , corresponde al pin RB7
 * FCY=60MIPS
 *Fecha:12/11/15
 */
#include <p33EP512GM304.h>
#include "MSPI1.h"
#include "IMU.h"
#include "RF.h" //Libreria nRF24l01 
#include "MemSPI.h"
#include "UARTSD.h"
#include <stdio.h> /* para usar  printf */
#include <string.h>
#define FCY 60000000UL//
#include <libpic30.h>
#include <uart.h>
#include <math.h>
#include <pps.h>

/*
 * Para memoria SD 
 */
#include "fileio.h"
#include "sd_spi.h"
extern FILEIO_SD_DRIVE_CONFIG sdCardMediaParameters;
const FILEIO_DRIVE_CONFIG gSdDrive = {
    (FILEIO_DRIVER_IOInitialize) FILEIO_SD_IOInitialize, // Function to initialize the I/O pins used by the driver.
    (FILEIO_DRIVER_MediaDetect) FILEIO_SD_MediaDetect, // Function to detect that the media is inserted.
    (FILEIO_DRIVER_MediaInitialize) FILEIO_SD_MediaInitialize, // Function to initialize the media.
    (FILEIO_DRIVER_MediaDeinitialize) FILEIO_SD_MediaDeinitialize, // Function to de-initialize the media.
    (FILEIO_DRIVER_SectorRead) FILEIO_SD_SectorRead, // Function to read a sector from the media.
    (FILEIO_DRIVER_SectorWrite) FILEIO_SD_SectorWrite, // Function to write a sector to the media.
    (FILEIO_DRIVER_WriteProtectStateGet) FILEIO_SD_WriteProtectStateGet, // Function to determine if the media is write-protected.
};
// Declare a state machine for our device

typedef enum {
    DEMO_STATE_NO_MEDIA = 0,
    DEMO_STATE_MEDIA_DETECTED,
    DEMO_STATE_DRIVE_MOUNTED,
    DEMO_STATE_DONE,
    DEMO_STATE_FAILED
} DEMO_STATE;

_FICD(ICS_PGD3 & JTAGEN_OFF); // Para hacer debuging por el puerto 2
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//_FOSCSEL(FNOSC_LPRC);
_FWDT(FWDTEN_OFF);
_FPOR(ALTI2C2_OFF); //  se usa I2C alternativo pines
//#pragma config FNOSC = LPRC  // default System clock = LPRC
/************************ HEADERS MIWI****************************************/

/* ** ********************Funciones****************************************************** ** */
void Config1(void); // Configuraci�b de puertos y otros
void IntInit(void);
void EnviarTX(void);
void EnviarMMA(void);
void MMA8471Activo(void);
void MMA8471DesActivo(void);
void MMA8471Reset(void);
void get_int_hex(unsigned int b, char *StrHexa); //Funcion para pasar de int Hexa a string
void GetTimestamp(FILEIO_TIMESTAMP * timeStamp); //Es para forzar fecha y hora en archivos SD
void readotro(char cod, char reg1, int count);
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void); // Rutina de interrupcion INT0
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void); //Rutina de interrupcion
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void); //Rutina de interrupcion;
void __attribute__((interrupt, no_auto_psv)) _SPI2Interrupt(void); //Rutina interrupcion SPI2
void char2intxyz(char entrada[6], signed int salida[3]);
/* ****************************************************************************************/


#define LED1	PORTBbits.RB0
/* ** *********************************************************************************** ** */
char cachito[32];
char cachito2[32];
signed char cachito2signed[3];
signed char circularsigned[3];
char tiempo[6]; //Vector para registro de tiempo para crear archivo en SD
unsigned char Vector[6]; //Vector para almacenar  Aceleraci�n
char VectorRS[256]; //Por ahora son 256 bytes
unsigned char VectorMEM[256]; //
unsigned char PageH, PageL; //es para almacenar en diferentes paginas en la memoria
char VectorMMA[192]; //
float VOffset[3];
int j, Trama;
unsigned int Cuenta = 0; // Registro de numero de vectores leidos
unsigned char ID;
int OUT_X = 0;
int OUT_Y = 0;
int OUT_Z = 0;
char buffer1[];
unsigned char datt;
unsigned char CTRL_REG1, Vcuenta = 0;
float temp = 0.0;
unsigned char orden, Kbhit, parar, Nbytes,RSB;
int Dato; //Variable de lectura del aceler�metro
char INT_SOURCE;
unsigned char Dire, DireH, DireL, Valor;
int datos[256] = {0}; //Vector de datos a escribir en la memoria
char aux, i;
char transmision[256] = {0};
char recepcion[256] = {0};
char comando_RF[256] = {0};
char l, k;
static const char nybble_chars[] = "0123456789ABCDEF"; //Vector constante de los simbolos hexadecimales
char Str[20]; //Vector para tener ascii de los enteros de las 3 aceleraciones
char Str1[5]; //Vector para tener ascii de los enteros de una aceleracion
unsigned int ENTERO; //Es para juntar las dos partes de la aceleracion en un entero 
char error_MAX_RT;
char N_dispositivo;
signed int cantidad_T = 0;
unsigned int cantidad_R = 0;
int m = 0;
char UltimaH = 0;
char UltimaL = 0;
long muestras_P, cuenta_M;
char muestras_C, indice_C, g, maestro_T;
int nivel, S, V;
signed char umbralchar;
char pipe_A, activo;
unsigned char Nombre[10]= "PLACA1";  //Vector para almacenar el nombre placa

union {
    unsigned char a[2];
    signed int vint;
} char2int;

union {
    unsigned char a[3];
    signed long vlong;
} char2long;
//direcciones de los pipes (invertidas, primero LSByte hasta MSByte)
char PIPE_0[5] = {0xe7, 0xe7, 0xe7, 0xe7, 0xe7};
char PIPE_1[5] = {0xc2, 0xc2, 0xc2, 0xc2, 0xc2};
char PIPE_2[5] = {0xc3, 0xc2, 0xc2, 0xc2, 0xc2};
char PIPE_3[5] = {0xc4, 0xc2, 0xc2, 0xc2, 0xc2};
char PIPE_4[5] = {0xc5, 0xc2, 0xc2, 0xc2, 0xc2};
char PIPE_5[5] = {0xc6, 0xc2, 0xc2, 0xc2, 0xc2};

char master = 0; //define si es maestro o esclavo
char N_pipe = 2; //define el pipe del esclavo


char circular[512];
//char circular[2520]__attribute__((eds)); //vector de almacenamiento circular de mediciones para modo trigger 420muestras

/***********************Registro de Jaime**********************************/


int main(void) {
    FILEIO_OBJECT file;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// CONDICIONES INICIALES y DECLARACIONES DE VARIABLES ////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //****************

    Config1(); // Configuraci�n de USART y mapeo de pines
    CS_FX = 1; //Levanto el CS del FX
    CS_M = 1; //Levanto el CS de la memoria serial 
    CS_RF = 1; //Levanto el CS del modulo RF 

    __delay_us(50); // A modo de prueba

    OpenSPI1(); //Configuro SPI 1
    // printf("Inicio Placa DSPIC\n");
    __delay_ms(5);

    OpenSPI2(); //Configuro SPI 2
    __delay_ms(10);

    OpenSPI3(); //Configuro SPI 3
    __delay_ms(10);

    //recupera  el numero de pipe guardado, si es mayor a 5 por defecto definimos el 1
    LeerPagina(0x7f, 0xfe, 0, VectorMEM);
    if (VectorMEM[254] > 5) {
        N_pipe = 1;
    } else {
        N_pipe = VectorMEM[254];
    }
    /*************************************
     configuraciones iniciales del nrf24L01+
     */
    //replicar los bytes altos de pipe 1 en 2 3 4 5
    for (j = 1; j < 5; j++) {
        PIPE_2[j] = PIPE_1[j];
        PIPE_3[j] = PIPE_1[j];
        PIPE_4[j] = PIPE_1[j];
        PIPE_5[j] = PIPE_1[j];
    }

    RFBWRITE(W_RF_SETUP, 0b00001100); //bajamos la potencia del transmisor -18dB
    //U1TXREG = RFBREAD(R_RX_PAYLOAD);
    RFBWRITE(W_SETUP_RETR, 0b01111111); //fijamos el tiempo entre retransmisiones en 250us y la cantidad maxima en 15


    /***********************************************************************
     Configuracion de la IMU
     ************************************************************************/

    IMUWRITE(0x2A, 0x0); // MMA se pone en modo des-activo
    __delay_ms(10); // tiempo necesario antes de iniciar otra comunicacion, en el caso de escritura

    /*********************Interrpci�n***************************************************/
    //Rutina de inicializaci�n de interrupciones
    IntInit();
    /* ***************I2C Inicializaci�n************************************************/
    //        Dato=IMUBREAD(cmdW,0x1A);//Leo registro 26 Estado de Interrupciones
    /* de paso bajo la bandera de dato listo en la IMU*/
    Kbhit = 0; // Limpio bandera RS
    parar = 0;

    RFBWRITE(W_FEATURE, 0b00000001); //habilitamos la orden de enviar sin ack

    UnlockMem();
    RFWRITE(FLUSH_TX, 0, 0); //FLUSH_TX que funciona :D
    RFWRITE(FLUSH_RX, 0, 0); //FLUSH_RX que funciona :D
    //ChipErase(); //borro la memoria (provisorio)
    if (master == 0) {
        RFBWRITE(W_CONFIG, 0b00111011); //PWR_UP=1, PRIM_RX=1,MASK_TX=1, MASK_RX=0,MASK_MAX_RT=1,
        __delay_us(150); //esperar el timepo Tpd2stby
        //RFBWRITE(W_EN_RXADDR, 0b00001000);//esclavo pipe 3 por ahora
        //U1TXREG = ((0b00000001 << N_pipe) | 0b00000001);
        aux = ((0b00000001 << N_pipe) | 0b00000001);
        //aux=0b00000001;
        RFBWRITE(W_EN_RXADDR, aux); //habilitacion del pipe correspondiente
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
        INTCON2bits.INT1EP = 1;
        IEC1bits.INT1IE = 1;


    }
    Kbhit = 0; // Limpio bandera llegada comunicaciones
    RSB=0;     //Limpio llegada comunicaciones RS
    parar = 0;
    while (1) {
        //U1TXREG = 0xcc;// codigo de procesamiento
        if (Kbhit == 1) {
            Kbhit = 0;
            switch (orden) {
                case 0x04://MODO TRIGGER FD,04,00,N�de maestro,umbral[2],muestras_P[3], muestras_C 
                    //borramos la memoria
                    ChipErase();
                    //todos los datos de mas de un byte los consireamos primero MSB, luego LSB
                    conf_recepcion_slave();
                    //asignar cantidad de muestras
                    char2long.a[0] = comando_RF[7];
                    char2long.a[1] = comando_RF[6];
                    char2long.a[2] = comando_RF[5];
                    muestras_P = char2long.vlong;
                    muestras_C = comando_RF[8]; //cantidad de muestras de la cola (viene en cantidad de paginas)

                    //guardamos el umbral
                    umbralchar = comando_RF[4];
                    //preparar el acelerometro
                    MMA8471DesActivo();
                    __delay_ms(1);
                    IMUWRITE(0x09, 0x00); //Fifo des habilitada
                    __delay_us(5);
                    IMUWRITE(0x2D, 0x01); //en CTRL_REG4 pongo INT_EN_DRDY=1
                    __delay_us(5);
                    MMA8471Activo();
                    __delay_us(5);
                    UnlockMem();
                    __delay_ms(1);
                    //fijamos el maestro
                    maestro_T = 0;
                    if (comando_RF[3] == N_pipe) {
                        maestro_T = 1; //yo soy el maestro
                        //mide las aceleraciones en el momento para fijar el estado de reposo
                        INT_SOURCE = IMUBREAD(0x0C); //leo INT_SOURCE
                        while ((INT_SOURCE & 0x01) == 0x01) {// pregunto si hay interrupcion por INT_EN_DRDY
                            INT_SOURCE = IMUBREAD(0x0C);
                        } //leo INT_SOURCE
                        for (j = 0; j < 6; j++) {
                            cachito2[j] = IMUBREAD(j + 1);
                            __delay_us(5);
                        }
                        cachito2signed[0] = cachito2[0]; //signed MSB x
                        cachito2signed [1] = cachito2[2]; //signed MSB y
                        cachito2signed[2] = cachito2[4]; //signed MSB Z

                    }
                    parar = 0;
                    Vcuenta = 0;

                    //almacenamiento ciclico de mediciones
                    while (parar != 5) {
                        INT_SOURCE = IMUBREAD(0x0C); //leo INT_SOURCE
                        if ((INT_SOURCE & 0x01) == 0x01)// pregunto si hay interrupcion por INT_EN_DRDY
                        {
                            for (j = 0; j < 6; j++) {
                                __delay_us(5);
                                circular[j + Vcuenta * 6] = IMUBREAD(j + 1); //guardo la cola en la variable circular                               
                            }
                            if (maestro_T == 1) {
                                //combinamos los datos medidos en signed int y lo comparamos con el almacenado

                                circularsigned[0] = circular[ Vcuenta * 6]; //MSB x estado actual
                                circularsigned[1] = circular[2 + Vcuenta * 6]; //MSB y estado actual
                                circularsigned[2] = circular[4 + Vcuenta * 6]; //MSB z estado actual
                                if (((circularsigned[0] > cachito2signed[0] + umbralchar) || (circularsigned[0] < cachito2signed[0] - umbralchar)) || ((circularsigned[1] > cachito2signed[1] + umbralchar) || (circularsigned[1] < cachito2signed[1] - umbralchar)) || ((circularsigned[2] > cachito2signed[2] + umbralchar) || (circularsigned[2] < cachito2signed[2] - umbralchar))) {
                                    parar = 5;
                                    VectorRS[0] = 0xFD;
                                    VectorRS[1] = 0x03; // comando para que se detengan los demas sensores
                                    VectorRS[2] = 0x01;
                                    RF_TX_32_NOACK(VectorRS, 0);
                                    RF_TX_32_NOACK(VectorRS, 0);
                                    RF_TX_32_NOACK(VectorRS, 0);
                                    conf_recepcion_slave();
                                }
                            }
                            Vcuenta = Vcuenta + 1;
                            if (Vcuenta > muestras_C * 42) {//reinicio  Vcuenta
                                Vcuenta = 0;
                                U1TXREG = 0x99;
                            }
                        }
                    }

                    indice_C = Vcuenta; //guardamos donde guardamos el ultimo dato en la memoria circular
                    Vcuenta = 0;
                    PageL = muestras_C; //empezamos a guardar nuevas mediciones dejando espacio para la cola
                    PageH = 0;
                    cuenta_M = 0;
                    UnlockMem();
                    while (cuenta_M < muestras_P)//almacenamos las aceleraciones muestras_P veces
                    {
                        INT_SOURCE = IMUBREAD(0x0C); //leo INT_SOURCE
                        if ((INT_SOURCE & 0x01) == 0x01)// pregunto si hay interrupcion por INT_EN_DRDY
                        {
                            for (j = 0; j < 6; j++) {
                                VectorMEM[j + Vcuenta * 6] = IMUBREAD(j + 1); //
                                __delay_us(5);
                            }
                            Vcuenta = Vcuenta + 1;
                            cuenta_M++;
                            if (Vcuenta == 42)//Representa 252 bytes quedan 4 bytes libre
                            {
                                VectorMEM[252] = PageH; //almaceno el valor alto de la pagina
                                VectorMEM[253] = PageL;
                                EscribirPagina(PageH, PageL, 0, VectorMEM); // escribo la pagina con datos del acelerometro       
                                PageL = PageL + 1;
                                if (PageL == 0) {
                                    PageH = PageH + 1;
                                }
                                Vcuenta = 0;
                            }
                        }
                    }

                    //escribimos la memoria ciclica en la mem flash
                    PageL = 0;
                    PageH = 0;
                    g = 0;
                    V = Vcuenta;
                    UnlockMem();
                    for (S = 0; S <= (muestras_C * 42); S++) {
                        memcpy(VectorMEM + g * 6, circular + V * 6, 6);
                        g++;
                        V++;
                        //completamos una pagina y la grabamos en memoria
                        if (g == 42) {
                            g = 0;
                            VectorMEM[252] = PageH; //almaceno el valor alto de la pagina
                            VectorMEM[253] = PageL;
                            EscribirPagina(PageH, PageL, 0, VectorMEM); // escribo la pagina con datos del acelerometro 
                            PageL = PageL + 1;
                            if (PageL == 0) {//cuando se desborda PageL suma 1 a PageH
                                PageH = PageH + 1;
                            }
                        }
                        if (V == (muestras_C * 42)) {
                            V = 0;
                        }
                    }
                    UltimaL = PageL;
                    UltimaH = PageH;
                    U1TXREG = 0xae;
                    break;

                case 0xff: //?

                    LeerPagina(comando_RF[3], comando_RF[4], 0, VectorMEM);
                    VectorMEM[0] = 0xab;
                    RF_TX_plus(VectorMEM, N_pipe, 256);
                    conf_recepcion_slave();
                    break;

                case 0x03://modo de tiempo fijo fd 06 cantidad_M[3] CHEQUEADO :D
                    //borramos la memoria
                    ChipErase();
                    //asignar cantidad de muestras
                    char2long.a[0] = comando_RF[5];
                    char2long.a[1] = comando_RF[4];
                    char2long.a[2] = comando_RF[3];
                    U1TXREG=comando_RF[5];
                    U1TXREG=comando_RF[4];
                    U1TXREG=comando_RF[3];
                    
                    muestras_P = char2long.vlong;

                    MMA8471DesActivo();
                    __delay_ms(1);
                    IMUWRITE(0x09, 0x00); //Fifo des habilitada
                    __delay_us(5);
                    IMUWRITE(0x2D, 0x01); //en CTRL_REG4 pongo INT_EN_DRDY=1
                    __delay_us(5);
                    MMA8471Activo();
                    __delay_us(5);
                    UnlockMem();
                    //al macenamiento de los datos
                    PageL = 0;
                    PageH = 0;
                    cuenta_M = 0;
                    Vcuenta = 0;
                    while (cuenta_M < muestras_P)//almacenamos las aceleraciones muestras_P veces
                    {
                        INT_SOURCE = IMUBREAD(0x0C); //leo INT_SOURCE
                        if ((INT_SOURCE & 0x01) == 0x01)// pregunto si hay interrupcion por INT_EN_DRDY
                        {
                            for (j = 0; j < 6; j++) {
                                VectorMEM[j + Vcuenta * 6] = IMUBREAD(j + 1); //
                                __delay_us(5);
                            }
                            Vcuenta++;
                            cuenta_M++;
                            if (Vcuenta == 42)//Representa 252 bytes quedan 4 bytes libre
                            {
                                VectorMEM[252] = PageH; //almaceno el valor alto de la pagina
                                VectorMEM[253] = PageL;
                                EscribirPagina(PageH, PageL, 0, VectorMEM); // escribo la pagina con datos del acelerometro       
                                PageL = PageL + 1;
                                if (PageL == 0) {
                                    PageH = PageH + 1;
                                }
                                Vcuenta = 0;
                            }
                        }
                    }
                    UltimaL = PageL;
                    UltimaH = PageH;
                    U1TXREG=0x13;
                    break;

                case ArchSD:// 0x10 Generar archivo SD 
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x4; // 4 valores
                    VectorRS[2] = orden;
                    // Register the GetTimestamp function as the timestamp source for the library.
                    FILEIO_RegisterTimestampGet(GetTimestamp);
                    if (FILEIO_Initialize() == -1) {
                        VectorRS[3] = 0x01; //error  de inicializacion de archivo
                        EnviarTX();
                        break; //finaliza proceso de grabar archivo en SD e indica error al soft
                    }
                    if (FILEIO_MediaDetect(&gSdDrive, &sdCardMediaParameters) == false) {
                        VectorRS[3] = 0x02; //SD no se detecta
                        EnviarTX();
                        break; //finaliza proceso de grabar archivo en SD e indica error al soft
                    }
                    if (FILEIO_DriveMount('A', &gSdDrive, &sdCardMediaParameters) != FILEIO_ERROR_NONE)
                        //Mount the SD card as a drive with drive letter 'A'    
                    {
                        VectorRS[3] = 0x03; //SD no se monta
                        EnviarTX();
                        break; //finaliza proceso de grabar archivo en SD e indica error al soft
                    }


                    // Open a file called  and store the file data in the FILEIO_OBJECT "file."
                    // This file is being opened with the CREATE flag, which will allow the library to create the file if it doesn't already exist, the
                    // TRUNCATE flag, which will truncate the file to a 0-length file if it already exists, the WRITE flag, which will allow us to
                    // write to the file, and the READ flag, which will allow us to read from it.

                    if (Valor == 1)//  comienzo pagina 0
                    {
                        PageH = 0;
                        if (FILEIO_Open(&file, "LECT1.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 2) {
                        PageH = 0x10; //  comienzo pagina 0x1000
                        if (FILEIO_Open(&file, "LECT2.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 3)//  comienzo pagina 0x2000
                    {
                        PageH = 0x20;
                        if (FILEIO_Open(&file, "LECT3.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 4)//  comienzo pagina 0x3000
                    {
                        PageH = 0x30;
                        if (FILEIO_Open(&file, "LECT4.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 5)//  comienzo pagina 0x4000
                    {
                        PageH = 0x40; //  
                        if (FILEIO_Open(&file, "LECT5.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 6)//  comienzo pagina 0x5000
                    {
                        PageH = 0x50; //  
                        if (FILEIO_Open(&file, "LECT6.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 7)//  comienzo pagina 0x6000
                    {
                        PageH = 0x60; // 
                        if (FILEIO_Open(&file, "LECT7.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }

                    if (Valor == 8)//  comienzo pagina 0x7000
                    {
                        PageH = 0x70; //  
                        if (FILEIO_Open(&file, "LECT8.CSV", FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE) {
                            VectorRS[3] = 0x04; //El archivo no se pudo crear
                            EnviarTX();
                            break; //finaliza proceso de grabar archivo en SD e indica error al soft
                        }
                    }
                    DireL = 0;
                    for (Cuenta = 0; Cuenta < 0x1000; Cuenta++)// leer 1Mbytes
                        /*
                        8 Mbytes de la memoria serial representa 0x8000 paginas de 256 byts cada uno 
                         La memoria tiene 128 sectores de 64 kbytes
                        ( 8 lecturas de 1 Mbytes  es 0x1000 paginass (decimal 4096)
                         * quedando cada medicion de 0x1000 pagina, que representa mas de 3,5 minutos
                         */ {
                        DireH = Cuenta >> 8;
                        DireL = Cuenta;
                        LeerPagina(DireH, DireL, 0, VectorMEM); //
                        for (j = 0; j < 42; j++)// Paso a ASCII los 42 valores
                        {
                            ENTERO = VectorMEM[6 * j]*256 + VectorMEM[6 * j + 1];
                            get_int_hex(ENTERO, Str1); //paso de int a ASCII en Hexa
                            strcat(Str, Str1);
                            ENTERO = VectorMEM[6 * j + 2]*256 + VectorMEM[6 * j + 3];
                            get_int_hex(ENTERO, Str1); //paso de int a ASCII en Hexa
                            strcat(Str, Str1);
                            ENTERO = VectorMEM[6 * j + 4]*256 + VectorMEM[6 * j + 5];
                            get_int_hex(ENTERO, Str1); //paso de int a ASCII en Hexa
                            strcat(Str, Str1);
                            strcat(Str, "\r\n");
                            FILEIO_Write(Str, 1, 17, &file); //escribo un renglon en el archivo
                            Str[0] = 0; //Forzamos en primer caracte en nulo
                        }
                        //        ENTERO=VectorMEM[252]*256+VectorMEM[253];    
                        //        get_int_hex( ENTERO,Str1 );//paso de int a ASCII en Hexa
                        //        strcat(Str, Str1 );
                        //        strcat(Str,"\r\n");
                        //        FILEIO_Write (Str, 1, 6, &file)  ;  //escribo un renglon en el archivo
                        //        Str[0]=0; //Forzamos en primer caracte en nulo
                        //       __delay_ms(10);//prueba
                    }

                    FILEIO_Close(&file);

                    VectorRS[3] = 0x05; //El archivo se creo con exito
                    EnviarTX();
                    break;


                case LEER_REGISTROS: //case 0x41 LEER_REGISTROS:
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x35; // 50 valores
                    VectorRS[2] = orden;
                    for (j = 0; j < 0x32; j++) {
                        VectorRS[j + 3] = IMUBREAD(j); // Leo todos los registros
                        __delay_us(5);
                    }
                    RF_TX_plus(VectorRS, N_pipe, VectorRS[1]);
                    conf_recepcion_slave();
                    break;

                case LEER_ACELERACIONES: //case 0x42  LEER_REGISTROS:
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x09; // 9 valores
                    VectorRS[2] = orden;
                    MMA8471Activo();
                    for (j = 1; j < 7; j++) {
                        VectorRS[j + 2] = IMUBREAD(j); //
                        __delay_us(5);
                    }
                    MMA8471DesActivo();
                    EnviarTX();
                    RF_TX_32(VectorRS, N_pipe);
                    __delay_ms(100);
                    conf_recepcion_slave();
                    break;
                    //hasta aca esta reemplazado el usart por el RF en el esclavo
                case ESC_REGISTRO: //case 0x43 escribir_REGISTROS:
                    MMA8471DesActivo();
                    __delay_ms(1);
                    IMUWRITE(Dire, Valor);
                    __delay_ms(1);
                    MMA8471Activo();
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;

                    //        for(j=1;j<7;j++)
                    //        {
                    //        VectorRS[j+2]=IMUBREAD(j);//
                    //        __delay_us(5);
                    //        }
                    //EnviarTX();
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();

                    break;
                    //hay que ver como se envia por RF (tiene mas de 32 bytes de respuesta)
                case LECT_FIFO: //CASE 0x44 Leer aceleraciones de la memoria FIFO 
                    parar = 1;
                    MMA8471DesActivo();
                    __delay_ms(1);
                    IMUWRITE(0x09, 0x80); //Fifo circular y 32 muestras
                    __delay_ms(1);
                    IMUWRITE(0x2D, 0x40); //en CTRL_REG4 pongo INT_EN_FIFO=1
                    __delay_ms(1);
                    IMUWRITE(0x2E, 0x40); //en CTRL_REG5 pongo Interrupt is routed to INT1 pin
                    __delay_ms(1);
                    IMUWRITE(0x2C, 0x82); // en CTRL_REG3 FIFO, interrupt active high (rising event)
                    __delay_ms(1);
                    //        IMUWRITE2(cmdW,0x2E,0x00);//  en CTRL_REG5 pongo Interrupt is routed to INT2 pin;
                    //        IMUWRITE2(cmdW,0x2F,0x00);//  en CTRL_REG3 pongo interrupcion en bajo y push-pull
                    MMA8471Activo();

                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0xc3; // 195 valores
                    VectorRS[2] = orden;

                    while (parar) {

                        if (PORTAbits.RA8) //Espero que se levante RA8
                        {
                            IMUBREAD(0x00); // Leo registro F Status
                            IMUREAD2(0x01, 192, VectorMMA); // Leo los 192 valores
                            EnviarMMA();
                            parar = 0; //Fin de rutina FIFO

                            MMA8471DesActivo();
                            __delay_ms(1);
                            IMUWRITE(0x09, 0x00); //Sin FIFO
                            __delay_ms(1);
                            IMUWRITE(0x2D, 0x00); //Sin Interrupciones
                            __delay_ms(1);
                            MMA8471Activo();
                            __delay_ms(1);
                        }
                    }
                    break;
                    //envia datos de forma contiua, hay que ver como hacemos el RF
                case LEER_ACEL_CONT: //CASE 0x45 Leer aceleraciones en forma continua
                    parar = 2;
                    MMA8471DesActivo();
                    __delay_ms(1);
                    IMUWRITE(0x09, 0x00); //Fifo des habilitada
                    __delay_ms(1);
                    IMUWRITE(0x2D, 0x01); //en CTRL_REG4 pongo INT_EN_DRDY=1
                    //        IMUWRITE2(cmdW,0x2E,0x00);//  en CTRL_REG5 pongo Interrupt is routed to INT2 pin;
                    //        IMUWRITE2(cmdW,0x2F,0x00);//  en CTRL_REG3 pongo interrupcion en bajo y push-pull
                    __delay_ms(1);
                    MMA8471Activo();
                    __delay_ms(1);
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 22; // 21 valores
                    VectorRS[2] = orden;
                    Vcuenta = 0;
                    Trama = 0;
                    while (parar == 2) {
                        INT_SOURCE = IMUBREAD(0x0C); //leo INT_SOURCE

                        if ((INT_SOURCE & 0x01) == 0x01)// pregunto si hay interrupcion por INT_EN_DRDY
                        {
                            for (j = 1; j < 7; j++) {
                                VectorRS[j + 2 + Vcuenta * 6] = IMUBREAD(j); //
                                __delay_us(5);
                            }
                            Vcuenta = Vcuenta + 1;
                            if (Vcuenta == 3) {
                                VectorRS[21] = Trama;
                                EnviarTX();
                                Vcuenta = 0;
                                Trama = Trama + 1;
                            }
                        }
                    }

                    break;

                case 0x46: //parar de leer
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 201 valores
                    VectorRS[2] = 0x46;
                    //EnviarTX();
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    MMA8471DesActivo();
                    IMUWRITE(0x09, 0x00); //des habilito la Fifo
                    __delay_ms(1);
                    IMUWRITE(0x2D, 0x00); //Sin Interrupciones
                    __delay_ms(1);
                    MMA8471Activo();
                    __delay_ms(1);

                    break;

                case ACEL_LOGGING_ON: //CASE 0x47 Almacenar en memoria la aceleracion
                    ChipErase(); //borramos la memoria
                    parar = 4;
                    UnlockMem();
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 3; // 3 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    MMA8471DesActivo();
                    __delay_ms(1);
                    IMUWRITE(0x09, 0x00); //Fifo des habilitada
                    __delay_us(5);
                    IMUWRITE(0x2D, 0x01); //en CTRL_REG4 pongo INT_EN_DRDY=1
                    //        IMUWRITE2(cmdW,0x2E,0x00);//  en CTRL_REG5 pongo Interrupt is routed to INT2 pin;
                    //        IMUWRITE2(cmdW,0x2F,0x00);//  en CTRL_REG3 pongo interrupcion en bajo y push-pull
                    __delay_us(5);
                    MMA8471Activo();
                    __delay_us(5);
                    PageL = 0;
                    Vcuenta = 0;
                    if (Valor == 0)PageH = 0; //  comienzo pagina 0
                    if (Valor == 1)PageH = 0x10; //  comienzo pagina 0x1000
                    if (Valor == 2)PageH = 0x20; //  comienzo pagina 0x2000
                    if (Valor == 3)PageH = 0x30; //  comienzo pagina 0x3000
                    if (Valor == 4)PageH = 0x40; //  comienzo pagina 0x4000
                    if (Valor == 5)PageH = 0x50; //  comienzo pagina 0x5000
                    if (Valor == 6)PageH = 0x60; //  comienzo pagina 0x6000
                    if (Valor == 7)PageH = 0x70; //  comienzo pagina 0x7000
                    ENTERO = 0;
                    while (parar == 4 && ENTERO < 0x1000)//Uso variable ENTERO para no gastar otra variable
                    {
                        INT_SOURCE = IMUBREAD(0x0C); //leo INT_SOURCE

                        if ((INT_SOURCE & 0x01) == 0x01)// pregunto si hay interrupcion por INT_EN_DRDY
                        {
                            for (j = 0; j < 6; j++) {
                                VectorMEM[j + Vcuenta * 6] = IMUBREAD(j + 1); //
                                __delay_us(5);
                            }
                            Vcuenta = Vcuenta + 1;

                            if (Vcuenta == 42)//Representa 252 bytes quedan 4 bytes libre
                            {
                                VectorMEM[252] = PageH; //almaceno el valor alto de la pagina
                                VectorMEM[253] = PageL;
                                EscribirPagina(PageH, PageL, 0, VectorMEM); // escribo la pagina con datos del acelerometro       
                                PageL = PageL + 1;
                                if (PageL == 0) {
                                    PageH = PageH + 1;
                                }
                                ENTERO = ENTERO + 1;
                                Vcuenta = 0;
                            }
                            //   if (PageH==1){parar=0;}//Por ahora termino con 80 segundos
                        }
                    }
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 3; // 3 valores
                    VectorRS[2] = orden;
                    //        VectorRS[3]=(ENTERO*0xff00)>>8;// 3 valores
                    //        VectorRS[4]=ENTERO*0x00ff;

                    break;

                case ACEL_LOGGING_OFF: //CASE 0x48 Parar de almacenar en memoria la aceleracion
                    if (parar == 3) {
                        VectorRS[0] = 0xFE;
                        VectorRS[1] = 0x5;
                        VectorRS[2] = orden;
                        VectorRS[3] = PageH; // envio las paginas almacenadas
                        VectorRS[4] = PageL;
                        UltimaL = PageL;
                        UltimaH = PageH;
                        RF_TX_32(VectorRS, N_pipe);
                        conf_recepcion_slave();
                        PageH = 0;
                        PageL = 0;
                        parar = 0;
                    } else if (parar == 0) {
                        VectorRS[0] = 0xFE;
                        VectorRS[1] = 0x5;
                        VectorRS[2] = orden;
                        VectorRS[3] = UltimaH; // envio las ultimas paginas almacenadas
                        VectorRS[4] = UltimaL;
                        RF_TX_32(VectorRS, N_pipe);
                        conf_recepcion_slave();
                    }
                    break;
                    //van a se rmuchos datos , hay que considerarlo
                case Leer_ACEL: //CASE 0x49 Leer datos Almacenados  en memoria 

                    LeerPagina(DireH, DireL, 0, VectorMEM);
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    while (U1STAbits.TRMT == 0);
                    U1TXREG = VectorRS[0];
                    while (U1STAbits.TRMT == 0);
                    U1TXREG = VectorRS[1];
                    while (U1STAbits.TRMT == 0);
                    U1TXREG = VectorRS[2];
                    for (j = 0; j < 256; j++) {
                        while (U1STAbits.TRMT == 0);
                        U1TXREG = VectorMEM[j];
                    }
                    //        PageH=0; // Por ahora comienzo pagina 0
                    //        PageL=0;
                    //        while(parar==4)
                    //        {
                    //       LeerPagina (int m, int n,int o,unsigned char *v[256]);   
                    //       EscribirPagina (PageH, PageL, 0, VectorMEM);// escribo la pagina con datos del acelerometro       
                    //       PageL=PageL+1;
                    //       if (PageL==0){PageH=PageH+1;}
                    //       
                    //   
                    //          if (PageH==1){parar=0;}//Por ahora termino con 80 segundos
                    //        }
                    break;

                case WhoamI: //CASE 0xa6 Quien soy yo
                   // U1TXREG = 0xba;
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x4; // 201 valores
                    VectorRS[2] = orden;
                    VectorRS[3] = IMUBREAD(0x0D); // Leo  el registro 0x0D
                   
                    //__delay_ms(100);
                    if(RSB==1) EnviarTX();  //Si mo es comunicacion seria es nRF
                    else  RF_TX_32(VectorRS, N_pipe);
                    //conf_recepcion_slave();
                    break;

                case LeerMemD: //case 0x50 leer un dato memoria serial
                    //      
                    VectorRS[3] = LeerDato(DireH, Dire, DireL); //Prueba eb la pagima 0x0100XX
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x04; // 4 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case EscMemD: //case 0x51 escribir un dato memoria serial
                    EscribirDato(DireH, Dire, DireL, Valor); //Prueba eb la pagima 0x0100XX
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case LeerMemID: //case 0x52 leer ID memoria serial
                    //EscribirDato (DireH,Dire,DireL, Valor);//Prueba eb la pagima 0x0100XX
                    ReadIdentification(&OUT_X, &OUT_Y, &OUT_Z);
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x06; // 3 valores
                    VectorRS[2] = orden;
                    VectorRS[3] = OUT_X;
                    VectorRS[4] = OUT_Y;
                    VectorRS[5] = OUT_Z;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case ResetMem: //case 0x53 reset memoria serial
                    //EscribirDato (DireH,Dire,DireL, Valor);//Prueba eb la pagima 0x0100XX
                    ChipErase();
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case LeerStatusMem: //case 0x54 Leer Status  memoria serial
                    LeerStatus1(&Dato);
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x04; // 3 valores
                    VectorRS[2] = orden;
                    VectorRS[3] = Dato;
                    // VectorRS[3]=LeerStatus();
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case EscStatusMem: //case 0x55 Esccribir Status  memoria serial
                    EscribirStatus(Valor);
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case UnlockProMem: //case 0x56 Unlock  memoria serial
                    UnlockMem(); //Global Block Protection Unlock
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case LeerConfMem: //case 0x57 Leer Reg Configuracion memoria serial
                    LeerConf1(&Dato);
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x04; // 3 valores
                    VectorRS[2] = orden;
                    VectorRS[3] = Dato;
                    // VectorRS[3]=LeerStatus();
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;
                    //hay que ver que haceeee pueden ser muchos valores
                case LeerPageMem: //case 0x58 Leer pagina memoria serial
                    LeerPagina(0, Dire, 0, VectorMEM); //Por ahora leo la pagina cero
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores, en realidad no, pero bueno
                    VectorRS[2] = orden;
                    while (U1STAbits.TRMT == 0);
                    U1TXREG = VectorRS[0];
                    while (U1STAbits.TRMT == 0);
                    U1TXREG = VectorRS[1];
                    while (U1STAbits.TRMT == 0);
                    U1TXREG = VectorRS[2];
                    for (j = 0; j < 256; j++) {
                        while (U1STAbits.TRMT == 0);
                        U1TXREG = VectorMEM[j];
                    }
                    break;
                    //en este vienen muchos datos :O 
                case EscPageMem: //case 0x59 Leer pagina memoria serial
                    for (j = 0; j < 256; j++) {
                        VectorMEM[j] = Valor + j;
                    }
                    EscribirPagina(0, Dire, 0, VectorMEM); //Por ahora escribo la pagina cero
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    EnviarTX();
                    break;

                case Reset: //case 0xA5 reset al acelerometro
                    MMA8471Reset();
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x03; // 3 valores
                    VectorRS[2] = orden;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;
                    //puede devolver mas de un paquete de 32bytes
                case MapanRF: //case 0x83 leo mapa de registro del nRF

                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x28; // 40 valores
                    VectorRS[2] = orden;
                    //Agregar lectura SPI3

                    CS_RF = 0; // Bajo el CS del nRF
                    //Para leer un registro R_REGISTER=000A AAAA

                    for (i = 0; i < 10; i++) {
                        CS_RF = 0;
                        WByteSPI3(i); //Leo distintos registros direccion 0 a 9   
                        VectorRS[i + 3] = RByteSPI3();
                        CS_RF = 1; // Levanto el CS del nRF
                    }
                    CS_RF = 0;
                    WByteSPI3(0x0A); //Leo  registro RX_ADDR_P0    
                    VectorRS[13] = RByteSPI3();
                    VectorRS[14] = RByteSPI3(); //
                    VectorRS[15] = RByteSPI3();
                    VectorRS[16] = RByteSPI3(); //	
                    VectorRS[17] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF 
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x0B); // Leo  registro RX_ADDR_P1  
                    VectorRS[18] = RByteSPI3();
                    VectorRS[19] = RByteSPI3(); //
                    VectorRS[20] = RByteSPI3();
                    VectorRS[21] = RByteSPI3(); //	
                    VectorRS[22] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF 
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x0C); //Leo  registro RX_ADDR_P2    
                    VectorRS[23] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x0D); //Leo  registro RX_ADDR_P3 
                    VectorRS[24] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x0E); //Leo  registro RX_ADDR_P4   
                    VectorRS[25] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x0F); //Leo  registro RX_ADDR_P5   
                    VectorRS[26] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x10); //Leo  registro TX_ADDR    
                    VectorRS[27] = RByteSPI3();
                    VectorRS[28] = RByteSPI3(); //
                    VectorRS[29] = RByteSPI3();
                    VectorRS[30] = RByteSPI3(); //	
                    VectorRS[31] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF 

                    for (i = 0x11; i < 0x18; i++) {
                        CS_RF = 0;
                        WByteSPI3(i); //Leo distintos registros direccion 17 a 23   
                        VectorRS[i + 15] = RByteSPI3();
                        CS_RF = 1; // Levanto el CS del nRF
                    }
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x1c); //Leo registro DYNPD 
                    VectorRS[39] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0;
                    WByteSPI3(0x1d); //Leo registro Feature 
                    VectorRS[40] = RByteSPI3();
                    CS_RF = 1; // Levanto el CS del nRF
                    EnviarTX();
                    break;


                case WReg1nRF:
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF
                    Dire = Dire | 0x20;
                    WByteSPI3(Dire); //Direccion
                    WByteSPI3(Valor); //Dato
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRF
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case wReg5nRF: //escribir cinco registro nRF. 0x86
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 35 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF
                    Vector[0] = Vector[0] | 0x20;
                    WByteSPI3(Vector[0]); //Direccion
                    WByteSPI3(Vector[1]); //Dato
                    WByteSPI3(Vector[2]); //Dato
                    WByteSPI3(Vector[3]); //Dato
                    WByteSPI3(Vector[4]); //Dato
                    WByteSPI3(Vector[5]); //Dato
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRFLeerFIFOnRF
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                    //devuelve un par de valores mas que 32                    
                case LeerFIFOnRF: //case 0x87 leo FIFO de registro del nRF
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x23; // 3 valores
                    VectorRS[2] = orden;

                    CS_RF = 0; // Bajo el CS del nRF
                    //Para leer un registro R_REGISTER=000A AAAA
                    WByteSPI3(0x61); //
                    for (i = 3; i < 35; i++) {
                        VectorRS[i] = RByteSPI3();
                    }
                    CS_RF = 1; // Levanto el CS del nRF
                    EnviarTX();
                    break;
                    //se puede recibir mas de 32      
                case EscFIFOnRFACK: //escribir FIFO con ACK nRF. 0x88
                    CS_RF = 0; // Bajo el CS del nRF
                    WByteSPI3(0xA0); //
                    for (i = 0; i < Nbytes - 3; i++) {
                        WByteSPI3(VectorRS[i]);
                    }
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(1);
                    CE_RF = 1;
                    __delay_us(20);
                    CE_RF = 0;
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    EnviarTX();
                    break;
                    //vielven un par de valores mas que 32  
                case RXnRF: //recepcion al RF. 0x89
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF 
                    WByteSPI3(0x00); //Leo  registro CONFIG    
                    datt = RByteSPI3();
                    datt = datt | 0x3; //POWER UP y RX/TX en RX en uno
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0; // Bajo el CS del nRF 
                    WByteSPI3(0x20); //Direcciono registro CONFIG
                    WByteSPI3(datt);
                    __delay_us(10);
                    CS_RF = 1; // Levanto el CS del nRF
                    CE_RF = 1; //Enciendo el equipo de RADIO
                    EnviarTX();
                    break;
                    //vielven un par de valores mas que 32   
                case TXnRF: //transmicion al nRF. 0x8A
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF 
                    WByteSPI3(0x00); //Leo  registro CONFIG    
                    datt = RByteSPI3();
                    datt = datt | 0x02; //POWER UP en 1 
                    datt = datt & 0xFE; // RX/TX en TX en cero
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0; // Bajo el CS del nRF 
                    WByteSPI3(0x20); //Direcciono registro CONFIG
                    WByteSPI3(datt);
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRF
                    CE_RF = 0; //Enciendo el equipo de RADIO
                    EnviarTX();
                    break;

                case SBynRF: //StandBy al RF. 0x8B
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF 
                    WByteSPI3(0x00); //Leo  registro CONFIG    
                    datt = datt & 0xFC; //POWER UP y RX/TX en RX en cero
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(10);
                    CS_RF = 0; // Bajo el CS del nRF 
                    WByteSPI3(0x20); //Direcciono registro CONFIG
                    WByteSPI3(datt);
                    __delay_us(10);
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(1);
                    CE_RF = 0; //Enciendo el equipo de RADIO
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;
                    //pueden llegar mas de 32
                case EscFIFOnRFNACK: //escribir FIFO sin ACK nRF. 0x8D

                    CS_RF = 0; // Bajo el CS del nRF
                    WByteSPI3(0xB0); //
                    for (i = 0; i < Nbytes - 3; i++) {
                        WByteSPI3(VectorRS[i]);
                    }
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRF
                    __delay_us(1);
                    CE_RF = 1;
                    __delay_us(20);
                    CE_RF = 0;
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    EnviarTX();
                    break;

                case CFIFORXnRF: //Limpiar FIFO RX nRF. 0x8E
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF
                    WByteSPI3(0xE2); //
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRF
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case CFIFOTXnRF: //Limpiar FIFO RX nRF. 0x8F
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x3; // 3 valores
                    VectorRS[2] = orden;
                    CS_RF = 0; // Bajo el CS del nRF
                    WByteSPI3(0xE1); //
                    __delay_us(1);
                    CS_RF = 1; // Levanto el CS del nRF
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    break;

                case PruINT: //case 0xAB Prueba de lectura de las aceleraciones con Interrupciones:
                    MMA8471DesActivo();
                    __delay_ms(10);
                    IMUWRITE(0x2D, 0x01); //en CTRL_REG4 pongo INT_EN_DRDY=1
                    IMUWRITE(0x2E, 0x01); //en CTRL_REG5 pongo INT_CFG_DRDY=1
                    MMA8471Activo(); //Ponemos al MMA en modo activo
                    IntInit(); //Habilito las interrupciones por INT1
                    break;

                default:
                    VectorRS[0] = 0xfe;
                    VectorRS[1] = 0xef;
                    RF_TX_32(VectorRS, N_pipe);
                    conf_recepcion_slave();
                    Nop();
                    break;
                case 0xfe:
                    N_pipe = comando_RF[3];
//borramos todooo
                    unsigned char estatus = 0x01;
                    EscrituraON();
                    while (estatus == 0x01) //veo si el  estatus es distinto de 0x01
                    {
                        estatus = LeerStatus()&0x01;
                    }
                    CS_M = 0;
                    WByteSPI2(0xC7); //Comando de borrado de toda la memoria
                    estatus = LeerStatus()&0x01;
                    while (estatus == 0x01) //veo si el  estatus es distinto de 0x01
                    {
                        estatus = LeerStatus()&0x01;
                    }
                    CS_M = 1;
                    UnlockMem();
                    estatus = LeerStatus()&0x01;
                    while (estatus == 0x01) //veo si el  estatus es distinto de 0x01
                    {
                        estatus = LeerStatus()&0x01;
                    }

                    UnlockMem();
                    memset(VectorMEM, N_pipe, 256);
                    //guargarlo en la memoria de programa o donde corresponda
                    //VectorMEM[3] = N_pipe;
                    EscribirPagina(0x7f, 0xfe, 0, VectorMEM);
                    U1TXREG = N_pipe;
                    conf_recepcion_slave();
                    break;
                case 0xfd:
                    U1TXREG = N_pipe;
                    LeerPagina(0x7f, 0xfe, 0, VectorMEM);
                    U1TXREG = VectorMEM[254];
                    conf_recepcion_slave();
                    break;
                    
                case Name:  //0xA7 envio el nombre de la placa
                    VectorRS[0] = 0xFE;
                    VectorRS[1] = 0x4; // 201 valores
                    VectorRS[2] = orden;
                    for (j = 0; j < 10; j++) {
                    VectorRS[j+3]=Nombre[j];    
                    }
                    //__delay_ms(100);
                    if(RSB==1) EnviarTX();  //Si mo es comunicacion seria es nRF
                    else  RF_TX_32(VectorRS, N_pipe);
                    //conf_recepcion_slave();
                    break;
                    

            }
        }
    }
}

/********************************************************************************/
void Config1(void)// Configuraci�b de puertos y otros
{
#define FCY 60000000UL//
#define BAUDRATE 19200
#define BRGVAL ((FCY/BAUDRATE)/4) - 1//con
    // Configure Oscillator to operate the device at 60 MHz
    // Fosc = Fin * M/(N1 * N2), Fcy = Fosc/2
    // Fosc = 4M * 120/(2 * 2) = 60 MHz for 4M input clock
    // Fosc = 8M * 60/(2 * 2) = 60 MHz for 8M input clock
    //valores frec para XTPLL hasta 8MHz , para valores mayores HS 
    PLLFBD = 58; //58;//118;			// M = 60
    CLKDIVbits.PLLPOST = 0; // N2 = 4
    CLKDIVbits.PLLPRE = 0; // N1 = 2
    /* Para tener Fcy=60Mhz
     Para cristal de 4Mhz hay que poner PLLPOST=0 , PLLPOST=0 y PLLFBD=118
     Para cristal de 8Mhz hay que poner PLLPOST=1 , PLLPOST=0 y PLLFBD=58
     */
    OSCTUN = 0; // Tune FRC oscillator, if FRC is used
    RCONbits.SWDTEN = 0; /* Disable Watch Dog Timer*/

    __builtin_write_OSCCONH(0x03); // Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONL(OSCCON | 0x01); //__builtin_write_OSCCONL(0x01);
    Nop();
    while (OSCCONbits.COSC != 0b011); // Wait for Clock switch to occur
    while (OSCCONbits.LOCK != 1) {
    }; // Wait for PLL to lock

    //	PORTB = 0;
    ANSELA = 0x0000; // Sin entradas anal�gicas
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    TRISAbits.TRISA9 = 1; //MISO SPI1     
    TRISCbits.TRISC3 = 0; // Clock SPI1
    TRISAbits.TRISA1 = 0; //MOSI SPI1    
    TRISAbits.TRISA8 = 1; //Entrada INT1 del aceler�metro
    TRISCbits.TRISC4 = 1; //Entrada INT2 del aceler�metro      
    TRISBbits.TRISB2 = 1; //MISO SPI2     
    TRISBbits.TRISB3 = 0; // Clock SPI2
    TRISCbits.TRISC1 = 0; //MOSI SPI2

    TRISBbits.TRISB10 = 1; //INTERRUPCION DE rf

    TRISCbits.TRISC9 = 1; //MISO SPI3
    TRISCbits.TRISC8 = 0; //clock SPI3
    TRISBbits.TRISB5 = 0; //MOSI SPI3

    TRISCbits.TRISC7 = 0; //CHIP ENABLE RF
    TRISBbits.TRISB6 = 0; //CSN
    CS_RF = 1; //


    TRISBbits.TRISB4 = 0; // CS del FX
    CS_FX = 1; // Levanto el CS del FX  
    TRISAbits.TRISA1 = 0; // CS de la memoria serial 
    TRISCbits.TRISC0 = 0; //CS SD SPI2
    CS_M = 1; // Levanto el CS de la memoria  

    TRISBbits.TRISB8 = 1; // RX
    TRISBbits.TRISB7 = 0; //TX

    TRISAbits.TRISA8 = 1; //Es para FX_INT1
    TRISCbits.TRISC4 = 1; //Es para FX_INT2

    TRISCbits.TRISC5 = 1; //como entrada (anulado)
    //	

    OSCCON = 0x46; // Command Sequence
    OSCCON = 0x57;
    OSCCONbits.IOLOCK = 0; // Peripherial pin select is not locked

    RPOR4bits.RP43R = 0b00001; // UART1_TX -> RP43 
    RPINR18bits.U1RXR = 0b101100; // UART_RX -> RP44

    // Configuraci�n de Pines para SPI2 , es para memoria serial
    RPOR5bits.RP49R = 0b00001000; // MOSI  SDO2 -> RP49
    RPOR0bits.RP35R = 0b00001001; // clock Out CLK2 -> RP35
    RPINR22bits.SDI2R = 0b00100010; // MISO  SDI2 -> RPI34 010 0011
    RPINR22bits.SCK2R = 0b00100011; // clock In CLK2 -> RP35 Por las dudas

    // Configuraci�n de Pines para SPI3 , es para el RF
    RPOR7bits.RP56R = 0b00100000; //clock del spi3 -> RP56
    RPOR1bits.RP37R = 0b00011111; //MOSI --> RP37
    RPINR0bits.INT1R = 0b00101010; //IRQ -> INT0 -> RP42
    RPINR29bits.SDI3R = 0b00111001; //MISO -> RP57
    RPINR29bits.SCK3R = 0b0111000; //clock del spi como entrada tambien




    // Transceiver Configuration

#define PHY_CS              LATBbits.LATB2
#define PHY_CS_TRIS         TRISBbits.TRISB2
#define PHY_RESETn          LATGbits.LATG2
#define PHY_RESETn_TRIS     TRISGbits.TRISG2
#define PHY_WAKE            LATGbits.LATG3
#define PHY_WAKE_TRIS       TRISGbits.TRISG3
    // SPI Configuration
#define SPI_SDI             PORTFbits.RF7
#define SDI_TRIS            TRISFbits.TRISF7
#define SPI_SDO             LATFbits.LATF8 
#define SDO_TRIS            TRISFbits.TRISF8
#define SPI_SCK             LATFbits.LATF6 
#define SCK_TRIS            TRISFbits.TRISF6

#define GetInstructionClock()	(CLOCK_FREQ/2)

    OSCCON = 0x46; // Command Sequence
    OSCCON = 0x57;
    OSCCONbits.IOLOCK = 1; // Peripherial pin select is locked

    // UART CONFIGURATION

    U1MODEbits.PDSEL = 0b00; // 8-bit data, no parity
    U1MODEbits.STSEL = 0; // One Stop bit
    U1MODEbits.ABAUD = 0; // Auto-Baud DISABLED
    U1MODEbits.BRGH = 1; // Low-speed mode
    U1BRG = BRGVAL; //   Valor inicial para XTPLL=86; Baud Rate - 19200 bps
    U1MODEbits.USIDL = 0; // Continue module operation in Idle mode
    U1MODEbits.IREN = 0; // IrDA� encoder and decoder disabled
    U1MODEbits.WAKE = 0; // No wake-up enabled
    U1MODEbits.LPBACK = 0; // Loopback mode is disabled
    U1MODEbits.URXINV = 0; // U1RX Idle state is '1'
    U1STAbits.UTXINV = 0; // UxTX Idle state is ?1?
    U1STAbits.OERR = 0; // Clear Overrun Error
    U1STAbits.ADDEN = 0; // Address Detect mode disabled
    U1STAbits.UTXBRK = 0; // Sync Break transmission disabled or completed
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 0;
    U1STAbits.URXISEL = 0;
    IFS0bits.U1RXIF = 0; // Limpio la bandera de la interrupci�n
    IEC0bits.U1RXIE = 1; //Habilito la interrupci�n por recepci�n RX
    U1MODEbits.UEN = 0; // UxTX and UxRX pins are enabled and used;
    // UxCTS and UxRTS/BCLK pins controlled by port latches
    U1MODEbits.UARTEN = 1; // UART1 is enabled
    U1STAbits.UTXEN = 1; // Transmit Enabled

}

/* ****************Interrupciones*********************************************/
void IntInit(void) {
    INTCON2bits.INT1EP = 1; //Flanco decreciente para INT1
    //   IFS0bits.INT0IF = 0;    /*Reset INT0 interrupt flag */
    //   IPC0=0x07;// Alta prioridad para INT0
    //   IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */
    //
    //   TRISBbits.TRISB7=1;// INT0 entrada
    IPC5bits.INT1IP = 3; // Nivel de prioridad de INT1
    IFS1bits.INT1IF = 0; //Limpio bandera
    IEC1bits.INT1IE = 1; //Habilito las interrupcion de INT1       
}

void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void) {

    IFS0bits.INT0IF = 0; //Clear the INT0 interrupt flag or else
    printf("s");

}

void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {
    //U1TXREG = 0xa1;
    IFS1bits.INT1IF = 0; //Clear the INT0 interrupt flag or else

    if (RFBREAD(R_STATUS)&0b01000000) {
        RFBWRITE(W_STATUS, 0b01110000); //bajamos las baderas internas del modulo
        RFREAD(R_RX_PAYLOAD, comando_RF + m * 32, 32);
        m++;
        pipe_A = (((RFBREAD(R_STATUS))& 0b00001110) >> 1); //si es cero es el pipe cero
        aux = (comando_RF[comando_RF[1] - 1]& (0b00000001 << N_pipe));
        activo = 1;
        if (pipe_A == 0) {//viene por pipe 0?
            if (aux == 0) {// si viene por pipe 0, le corresponde escuchar?
                activo = 0;
            }
            if (comando_RF[2] == 0xfe) { //es un comando de configuracion de pipe?
                activo = 1;
            }
        }
        if (activo == 1) {
            if (cantidad_R == 0) {
                orden = comando_RF[2];
                U1TXREG = orden;
            }
            cantidad_R = cantidad_R + 32;
            if (cantidad_R >= comando_RF[1]) {
                Kbhit = 1;
                cantidad_R = 0;
                m = 0;
                switch (orden) {
                    case 0x01:
                        parar = 5;
                        Kbhit = 0;
                        U1TXREG = 0x01;
                        break;
                    case 0x47://Si llega la orden 0x47 inicio de lectura e indica cual de las 8 lectura es
                        Valor = comando_RF[3]; //Indica que lectura es
                        break;
                    case 0x48://Si llega la orden 0x46 o 0x48 deja de enviar o almacenar datos
                        if (parar == 4) {
                            parar = 3;
                        } else if (parar == 3 || parar == 0) {
                            parar = 0;
                        }
                        break;
                }
                RFWRITE(FLUSH_TX, 0, 0); //FLUSH_TX que funciona :D
                RFWRITE(FLUSH_RX, 0, 0); //FLUSH_RX que funciona :D
            }
        }
    }
    conf_recepcion_slave();
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    int dato;
    int m = 0, l = 0;
    char contador = 0;
    //while (!DataRdyUART1());
    dato = U1RXREG;
    
    if (master == 1) {
        for (m = 0; m < 32; ++m) {
            transmision[m] = 0;
        }
        N_dispositivo = 0;
        if (dato == 0xFD) {
            transmision[0] = dato;
            while (!DataRdyUART1());
            transmision[1] = U1RXREG;
            transmision[1]--;
            cantidad_T = transmision[1];
            while (!DataRdyUART1());
            N_dispositivo = U1RXREG;
            //N_dispositivo = 3;
            while (contador < (transmision[1] - 2)) {
                while (!DataRdyUART1());
                transmision[contador + 2] = U1RXREG;
                contador++;
            }
            if (N_dispositivo == 0) {
                RF_TX_32_NOACK(transmision, N_dispositivo);
            } else {
                m = 0;
                while (cantidad_T > 0) {
                    RF_TX_32(transmision + m * 32, N_dispositivo);
                    cantidad_T = cantidad_T - 32;
                    m++;
                }
                //RF_TX_32(transmision, N_dispositivo); //aca decidimos con que sensor comunicarme 
                if (error_MAX_RT == 1) {
                    U1TXREG = 0XD3; //error por maximas retransmisiones   
                } else {
                    //recepcion de datos
                    //primeros 32
                    RF_RX_32(recepcion);
                    l++;
                    cantidad_R = recepcion[1];
                    cantidad_R = cantidad_R - 32;
                    //resto de los datos
                    while (cantidad_R > 0) {
                        cantidad_R = cantidad_R - 32;
                        RF_RX_32(recepcion + l * 32);
                        l++;
                    }
                    l = 0;
                    // VectorRS[0] = 0xFE;
                    //VectorRS[1] = recepcion[1];
                    for (j = 0; j < recepcion[1]; j++) {
                        VectorRS[j] = recepcion[j];
                    }
                    EnviarTX();
                }
            }
        }
    } else {


        if (dato == 0xFD) {
            while (!DataRdyUART1());
            dato = U1RXREG;
            while (!DataRdyUART1());
            orden = U1RXREG;
            switch (orden) {
                case 0x10://Si llega la orden 0x10 inicio la conversion de la memoria seria ala SD e indica cual de las 8 lectura es
                {
                    while (!DataRdyUART1());
                    Valor = U1RXREG; //Indica que lectura es

                    while (!DataRdyUART1());
                    tiempo[0] = U1RXREG; //Indica el mes

                    while (!DataRdyUART1());
                    tiempo[1] = U1RXREG; //Indica el dia

                    while (!DataRdyUART1());
                    tiempo[2] = U1RXREG; //Indica el ano

                    while (!DataRdyUART1());
                    tiempo[3] = U1RXREG; //Indica la hora

                    while (!DataRdyUART1());
                    tiempo[4] = U1RXREG; //Indica los minutos

                    while (!DataRdyUART1());
                    tiempo[5] = U1RXREG; //Indica los segundos
                    break;
                }

                case 0x43:// llegan 5 bytes
                {
                    while (!DataRdyUART1());
                    Dire = U1RXREG;
                    while (!DataRdyUART1());
                    Valor = U1RXREG;
                    break;
                }

                case 0x46://Si llega la orden 0x46 o 0x48 deja de enviar o almacenar datos
                {
                    parar = 0;
                    break;
                }

                case 0x47://Si llega la orden 0x47 inicio de lectura e indica cual de las 8 lectura es
                {
                    while (!DataRdyUART1());
                    Valor = U1RXREG; //Indica que lectura es
                    break;
                }
                case 0x48://Si llega la orden 0x46 o 0x48 deja de enviar o almacenar datos
                {
                    parar = 3;
                    break;
                }
                case 0x49://Si llega la orden 0x49 indico que pagina leo
                {
                    while (!DataRdyUART1());
                    DireH = U1RXREG;
                    while (!DataRdyUART1());
                    DireL = U1RXREG;

                    break;
                }
                case 0x50:// llegan 6 bytes por ahora
                {
                    while (!DataRdyUART1());
                    DireH = U1RXREG;
                    while (!DataRdyUART1());
                    Dire = U1RXREG;
                    while (!DataRdyUART1());
                    DireL = U1RXREG;
                    break;
                }

                case 0x51:// llegan 7 bytes
                {
                    while (!DataRdyUART1());
                    DireH = U1RXREG;
                    while (!DataRdyUART1());
                    Dire = U1RXREG;
                    while (!DataRdyUART1());
                    DireL = U1RXREG;
                    while (!DataRdyUART1());
                    Valor = U1RXREG;
                    break;
                }

                case 0x55: //  llegan 4 bytes
                {
                    while (!DataRdyUART1());
                    Valor = U1RXREG;
                    break;
                }

                case 0x58: //  llegan 4 bytes
                {
                    while (!DataRdyUART1());
                    Dire = U1RXREG;
                    break;
                }

                case 0x59: //  llegan 5 bytes
                {
                    while (!DataRdyUART1());
                    Dire = U1RXREG;
                    while (!DataRdyUART1());
                    Valor = U1RXREG;
                    break;
                }

                case 0x85: //  llegan 5 bytes pata nRF
                {
                    while (!DataRdyUART1());
                    Dire = U1RXREG;
                    while (!DataRdyUART1());
                    Valor = U1RXREG;
                    break;
                }
                case 0x86: //  llegan 9 bytes pata nRF
                {
                    while (!DataRdyUART1());
                    Vector[0] = U1RXREG;
                    while (!DataRdyUART1());
                    Vector[1] = U1RXREG;
                    while (!DataRdyUART1());
                    Vector[2] = U1RXREG;
                    while (!DataRdyUART1());
                    Vector[3] = U1RXREG;
                    while (!DataRdyUART1());
                    Vector[4] = U1RXREG;
                    while (!DataRdyUART1());
                    Vector[5] = U1RXREG;
                    break;
                }
                case 0x88: //  llegan Nbytes bytes pata nRF
                {
                    for (j = 0; j < Nbytes - 3; j++) {
                        while (!DataRdyUART1());
                        VectorRS[j] = U1RXREG;
                    }
                    break;
                }
                case 0x8D: //  llegan 35 bytes pata nRF
                {
                    for (j = 0; j < Nbytes - 3; j++) {
                        while (!DataRdyUART1());
                        VectorRS[j] = U1RXREG;
                    }
                    break;
                }
                case 0xfe:
                    while (!DataRdyUART1());
                    DireH = U1RXREG;
                    while (!DataRdyUART1());
                    DireL = U1RXREG;
                    break;
                  
                
                    
                default:
                    Nop();

                    break;

            }
            Kbhit = 1;
            RSB=1;//Bandera de llegada pregunta RS232
        }
        IFS0bits.U1RXIF = 0; // Clear RX Interrupt flag
    }
}

void EnviarTX(void) {

    unsigned int i, cant;
    cant = VectorRS[1];
    for (i = 0; i < cant; i++) {

        while (U1STAbits.TRMT == 0);
        //while(IFS0bits.U1TXIF);
        U1TXREG = VectorRS[i];
        Nop();
        IFS0bits.U1TXIF = 0;
    }
    RSB=0; //limpio bandera de llegada
}

void MMA8471Activo(void) {

    CTRL_REG1 = IMUBREAD(0x2A); //Leo el registro 0x2A
    CTRL_REG1 = CTRL_REG1 | 0x01; //seteo el bit ACTIVE
    __delay_ms(1);
    IMUWRITE(0x2A, CTRL_REG1); // MMA se pone en modo activo
    //     IMUWRITE(0x2A,0x01);// MMA se pone en modo desactivo
}

void MMA8471DesActivo(void) {

    CTRL_REG1 = IMUBREAD(0x2A); //Leo el registro 0x2A
    CTRL_REG1 = CTRL_REG1 & 0xFE; //Limpio el bit ACTIVE
    __delay_ms(1);
    IMUWRITE(0x2A, CTRL_REG1); // MMA se pone en modo desactivo
    //     IMUWRITE(0x2A,0x00);// MMA se pone en modo desactivo
}

void MMA8471Reset(void)//Genero un Reset en el acelerometro
{

    CTRL_REG1 = IMUBREAD(0x2B); //Leo el registro CTRL_REG2 0x2B
    CTRL_REG1 = CTRL_REG1 | 0x40; //seteo el bit rst 
    __delay_ms(1);
    IMUWRITE(0x2B, CTRL_REG1); // Seteo el bit RST de registro CTRL_REG2
    __delay_ms(1);
    CTRL_REG1 = IMUBREAD(0x2B); //Leo el registro CTRL_REG2 0x2B
    CTRL_REG1 = CTRL_REG1 & 0xBF; //Limpio el bit rst 
    __delay_ms(1);
    IMUWRITE(0x2B, CTRL_REG1); // Limpio el bit RST de registro CTRL_REG2
    __delay_ms(1);
    MMA8471Activo();

}

void EnviarMMA(void) {
    unsigned int i;
    for (i = 0; i < 3; i++) {
        U1TXREG = VectorRS[i];
        while (!U1STAbits.TRMT);
    }
    for (i = 0; i < 192; i++) {
        U1TXREG = VectorMMA[i];

        while (!U1STAbits.TRMT);

    }

}

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
}

void __attribute__((interrupt, no_auto_psv)) _SPI2Interrupt(void) {

    Nop();
    IFS2bits.SPI2IF = 0;
}

//void GetTimestamp (FILEIO_TIMESTAMP * timeStamp)//Es para poner fecha y hora en archivo SD
//{
//timeStamp->timeMs = 0;
//timeStamp->time.bitfield.hours = 0;
//timeStamp->time.bitfield.minutes = 0;
//timeStamp->time.bitfield.secondsDiv2 = 0;
//timeStamp->date.bitfield.day = 8;
//timeStamp->date.bitfield.month = 1;
//timeStamp->date.bitfield.year = 37;//Year (number of years since 1980)
//}

void GetTimestamp(FILEIO_TIMESTAMP * timeStamp)//Es para poner fecha y hora en archivo SD
{

    timeStamp->timeMs = 0;
    timeStamp->time.bitfield.hours = tiempo[3];
    timeStamp->time.bitfield.minutes = tiempo[4];
    timeStamp->time.bitfield.secondsDiv2 = tiempo[5];
    timeStamp->date.bitfield.day = tiempo[1];
    timeStamp->date.bitfield.month = tiempo[0];
    timeStamp->date.bitfield.year = 20 + tiempo[2];
    ; //Year (number of years since 1980)
}

void char2intxyz(char entrada[6], signed int salida[3]) {
    char h;
    for (h = 0; h < 3; h++) {
        char2int.a[1] = entrada[h * 2];
        char2int.a[0] = entrada[h * 2 + 1];
        salida[h] = char2int.vint;
    }
}

void get_int_hex(unsigned int b, char *StrHexa)//Funcion para pasar de int Hexa a string
{
    *StrHexa = nybble_chars[ (b >> 12) & 0x0F ];
    StrHexa++;
    *StrHexa = nybble_chars[ (b >> 8) & 0x0F ];
    StrHexa++;
    *StrHexa = nybble_chars[ (b >> 4) & 0x0F ];
    StrHexa++;
    *StrHexa = nybble_chars[ b & 0x0F ];
    StrHexa++;
    *StrHexa = ';'; //separaci9on de punto y coma
}
//temas por ver:
//casos en que la respuesta sea mas de 255
//caso de lectura continua
//como le decimos al esclavo que pipe le corresponde (desde afuera del programa)
//ver excepciones, errores de comunicacion y manejarlos
//hacer que mas instrucciones funcionen por RF (que contesten por RF)
//probar SD
//
