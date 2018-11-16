#ifndef IMU_H // header guards
#define IMU_H
#define FCY 60000000UL//
#include <libpic30.h>
unsigned char IMUBREAD(char reg);
void IMUREAD(char reg, int count, unsigned char array[]);
void IMUWRITE(char reg, char value);// para IMU
unsigned char GET_STATUS(void);
unsigned char GET_CTRL_REG1(void);
void GET_OUT_XYZ(int *OUT_X, int *OUT_Y, int *OUT_Z);
void GET_OUT_XYZ2(int *OUT_X, int *OUT_Y, int *OUT_Z);
void SET_SCALE(unsigned char scale);
void IMUREADGYR(char cod,float arrayf[]);

#define  FX_INT1    LATAbits.LATA8
#define  FX_INT2    LATCbits.LATC4

typedef enum
{
    ArchSD              =0x10,// Crear archivo memoria SD
    RECEPTOR            = 0x3F,
    TRANSMISOR          = 0x40,
    LEER_REGISTROS      = 0x41,//"A"
    LEER_ACELERACIONES  = 0x42, //"B"
    ESC_REGISTRO        = 0x43, //"C"
    LECT_FIFO           = 0x44, //"D"
    LEER_ACEL_CONT      = 0x45, //"E"
    RD_ACEL             = 0x46, //"F"
    ACEL_LOGGING_ON     = 0x47, //"G"
    ACEL_LOGGING_OFF    = 0x48, //"H"
    Leer_ACEL           = 0x49, //"I"
    CONTROL             = 0x4A, //"J"
    LeerMem             = 0x4B, //"K"
    LeerMemOff          = 0x4C, //"L"
    WhoamI              = 0xA6, //Quien soy yo acelerómetro SOY TU PADRE, NOOOOOOOOOOOOOOOOOO
    LeerMemD            = 0x50,  //Comando para leer un dato en la memorai serial
    EscMemD             = 0x51,  //Comando para escribir un dato en la memorai serial 
    LeerMemID           = 0x52,  //Comando para leer el ID de la memoria
    ResetMem            = 0x53,  //Comando para limpiar toda la memoria
    LeerStatusMem       = 0x54,  //Comando para leer el estatus de la memoria
    EscStatusMem        = 0x55,  //Comando para escribir el estatus de la memoria
    UnlockProMem        = 0x56,  //Comando para unlock  la memoria  
    LeerConfMem         = 0x57,  //Comando para leer el registro configuracion de la memoria
    LeerPageMem         = 0x58,  //Comando para leer una pagina  de la memoria
    EscPageMem          = 0x59,  //Comando para escribir una pagina  de la memoria       
    MapanRF             = 0x83,  //Leo Mapa de registro del nRF
    WReg1nRF            = 0x85, //escrino un registro en el mapa del nRF        
    wReg5nRF            = 0x86, //escribo cinco registros al nRF
    LeerFIFOnRF         = 0x87, //leo registros de la FIFO al nRF  
    EscFIFOnRFACK       = 0x88, //escribir registros de la FIFO al nRF con ACK
    RXnRF               = 0x89, //Recepcion al nRF 
    TXnRF               = 0x8A, //Transmicion  al nRF 
    SBynRF              = 0x8B, //StandBy al nRF 
    EscFIFOnRFNACK      = 0x8D, //escribir registros de la FIFO al nRF sin ACK
    CFIFORXnRF          = 0x8E, //Limpiar FIFO RX al nRF 
    CFIFOTXnRF          = 0x8F, //Limpiar FIFO TX al nRF 
    Reset               = 0xA5,   //Envío un reset al acelerómetro
    PruINT              = 0xAB,  //Prueba de funcionamiento de Interrupcion del acelerometro  
    Name                = 0xAC  //Informa el nombre de la placa
}TYPE_CMD;



#endif