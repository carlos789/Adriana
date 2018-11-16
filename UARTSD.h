#ifndef UI_LABVIEW_H // header guards
#define UI_LABVIEW_H
#include  <stdint.h>//Es para tomar denominaciones de variables
#define ConsoleIsPutReady()     (U2STAbits.TRMT) 
#define ConsoleIsGetReady()     (IFS1bits.U2RXIF)

void write_float (float variable);
void read_float (char *variable);
void syncro (void);
void ui_labview(int *OUT_X, int *OUT_Y, int *OUT_Z, float *temp);
void write_entero (int variable);
void write_vector (int count, char *array);
void ConsoleInit(void);


#endif
