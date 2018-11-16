#ifndef MemSPI_H // header guards
#define MemSPI_H
#define FCY 60000000UL//
#include <libpic30.h>
extern unsigned char VectorMEM[256];//Definida en el main
void Delay5mseg ( void );
void Delay ( void );
void EscrituraON (void);
void EscribirStatus (unsigned char valor);
 unsigned char LeerStatus (void);
//void EscribirPagina (int f, int g, int h);
void EscribirPagina (int f, int g, int h, unsigned char* v[256]);
void LeerPagina (int m, int n,int o,unsigned char *v[256]);
unsigned int SPIRead (void);
void ReadIdentification (int* a,int* b,int* c);
void ReadRES (int* d);
void Delay05mseg ( void );
void SectorErase(void);
void ChipErase(void);
void EscribirPaginaIndex (int f, int g, int h,int r, int* v[256]);
void EscribirDato (unsigned char datoHH,unsigned char datoH,unsigned char datoL,unsigned char DATO);
unsigned char LeerDato (unsigned char datoHH,unsigned char datoH,unsigned char datoL);
unsigned char LeerStatus1 (int* a);
unsigned char LeerConf1 (int* a);
void UnlockMem (void);  //Global Block Protection Unlock

#endif
