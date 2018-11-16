#ifndef MI2C_H // header guards
#define MI2C_H

void OpenI2C1(void);
void StartI2C1(void);
void RestartI2C1(void);
void StopI2C1(void);
void AckI2C1(void);
void WaitAckI2C1(void);
void NotAckI2C1(void);
unsigned char DataRdyI2C1(void);
void WriteI2C1(unsigned char data_out);
void OpenI2C2(void);
void StartI2C2(void);
void RestartI2C(void);
void StopI2C2(void);
void AckI2C2(void);
void WaitAckI2C2(void);
void NotAckI2C2(void);
unsigned char DataRdyI2C2(void);
void WriteI2C2(unsigned char data_out);
#endif
