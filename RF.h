//Funciones
void RFREAD(char comando,char *SPI_IN, unsigned char cantidad);
unsigned char RFBREAD(char reg);
void RFBWRITE(char reg, char value);
void RFWRITE(char registro, char *SPI_OUT,unsigned char cantidad);
void RF_TX(char data);
char RF_RX();
char RF_RX_32 (char *payloadRX);
char RF_TX_plus (char *payloadRX,char pipe,int cantidad_datos);
char RF_TX_32 (char *payloadTX,char pipe);
char RF_TX_32_NOACK(char * payloadTX,char pipe);
char cambiar_pipe(char pipe);
char conf_recepcion_slave();
//char RF_RX_32 ();
//char RF_TX_32 ();
//void RFREAD(char comando, unsigned char cantidad);
//void RFWRITE(char registro,unsigned char cantidad);
//fin de Funciones

#define     PTX     0
#define     PRX     1


//Comandos
#define     R_RX_PAYLOAD            0b01100001
#define     W_TX_PAYLOAD            0b10100000
#define     FLUSH_TX                0b11100001
#define     FLUSH_RX                0b11100010
#define     REUSE_TX_PL             0b11100011
#define     R_RX_PL_WID             0b01100000
#define     W_TX_PAYLOAD_NO_ACK      0b10110000
#define     W_ACK_PAYLOAD_0         0b10101000
#define     W_ACK_PAYLOAD_1         0b10101001
#define     W_ACK_PAYLOAD_2         0b10101010
#define     W_ACK_PAYLOAD_3         0b10101011
#define     W_ACK_PAYLOAD_4         0b10101100
#define     W_ACK_PAYLOAD_5         0b10101101
#define     NOP                     0b11111111

//fin de Comandos

//Resgistros escritura
#define     R_CONFIG                0b00000000
#define     R_EN_AA                 0b00000001
#define     R_EN_RXADDR             0b00000010
#define     R_SETUP_AW              0b00000011
#define     R_SETUP_RETR            0b00000100 
#define     R_RF_CH                 0b00000101
#define     R_RF_SETUP              0b00000110
#define     R_STATUS                0b00000111
#define     R_OBSERVE_TX            0b00001000
#define     R_RPD                   0b00001001
#define     R_RX_ADDR_P0            0b00001010
#define     R_RX_ADDR_P1            0b00001011
#define     R_RX_ADDR_P2            0b00001100
#define     R_RX_ADDR_P3            0b00001101
#define     R_RX_ADDR_P4            0b00001110
#define     R_RX_ADDR_P5            0b00001111
#define     R_TX_ADDR               0b00010000
#define     R_RX_PW_P0              0b00010001
#define     R_RX_PW_P1              0b00010010
#define     R_RX_PW_P2              0b00010011
#define     R_RX_PW_P3              0b00010100
#define     R_RX_PW_P4              0b00010101
#define     R_RX_PW_P5              0b00010110
#define     R_FIFO_STATUS           0b00010111
#define     R_DYNPD                 0b00011100
#define     R_FEATURE               0b00011101

//Resgistros LECTURA
#define     W_CONFIG                0b00100000
#define     W_EN_AA                 0b00100001
#define     W_EN_RXADDR             0b00100010
#define     W_SETUP_AW              0b00100011
#define     W_SETUP_RETR            0b00100100 
#define     W_RF_CH                 0b00100101
#define     W_RF_SETUP              0b00100110
#define     W_STATUS                0b00100111
#define     W_OBSERVE_TX            0b00101000
#define     W_RPD                   0b00101001
#define     W_RX_ADDR_P0            0b00101010
#define     W_RX_ADDR_P1            0b00101011
#define     W_RX_ADDR_P2            0b00101100
#define     W_RX_ADDR_P3            0b00101101
#define     W_RX_ADDR_P4            0b00101110
#define     W_RX_ADDR_P5            0b00101111
#define     W_TX_ADDR               0b00110000
#define     W_RX_PW_P0              0b00110001
#define     W_RX_PW_P1              0b00110010
#define     W_RX_PW_P2              0b00110011
#define     W_RX_PW_P3              0b00110100
#define     W_RX_PW_P4              0b00110101
#define     W_RX_PW_P5              0b00110110
#define     W_FIFO_STATUS           0b00110111
#define     W_DYNPD                 0b00111100
#define     W_FEATURE               0b00111101