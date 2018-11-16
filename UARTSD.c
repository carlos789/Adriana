#include <uart.h>
//No se usan estas funciones, quedan por las dudas
void write_float (float variable){			// saco Float por la USART
	
	int i;
	char* p = &variable;					//Puntero char a Ts
         //Permite individualizar cada byte de los 4 que hacen al numero en punto flotante
	while(BusyUART1());
	i=0;
	while (i<4){
		WriteUART1(*(p+3-i));
		while(BusyUART1());
		i++;
	}										//  fin saco float por la USART
}
void write_entero (int variable){			// saco Float por la USART
	
	int i;
	char* p = &variable;					//Puntero char a Ts
           									//Permite individualizar cada byte de los 4 que hacen al numero en punto flotante
	while(BusyUART1());
	i=0;
	while (i<2){
		WriteUART1(*(p+i));
		while(BusyUART1());
		i++;
	}										//  fin saco float por la USART
}
void write_vector (int count, char *array){			// saco Float por la USART

	int i;
	//char* p = &variable;					//Puntero char a Ts
           									//Permite individualizar cada byte de los 4 que hacen al numero en punto flotante
	while(BusyUART1());
	i=0;
	while (i<count){
		WriteUART1(array[i]);
		while(BusyUART1());
		i++;
	}        //  fin sale Vector USART
        while(BusyUART1());
	putcUART1(0x0A);//Salto de linea
	while(BusyUART1());
	putcUART1(0x0D);
}
void read_float (char *variable){			// recibo Float por la USART - El parametro es la direccion de la varaible
											// char *variable es un puntero char, cuando se invoca la funcion, apunta a un float
											// Permite individualizar cada byte de los 4 que hacen al numero en punto flotante
	int i;
	i=0;									//Recibir los 4 bytes de variable
	while(i<4){
		while (!DataRdyUART1());
		*(variable+3-i)=ReadUART1();		//Usa el puntero char para construir el float byte a byte
		i++;
	}
}

void syncro (void){
	char sync;

START:	
	sync=' ';
	while(!DataRdyUART1());
	sync = ReadUART1();
	if(sync != 'S')		goto START;
		
	sync = ' ';
	while(!DataRdyUART1());
	sync = ReadUART1();
	if(sync != 'T')		goto START;

	while(BusyUART1());
	putcUART1('O');
	while(BusyUART1());
	putcUART1('K');
	while(BusyUART1());
}


void ui_labW(int *OUT_X, int *OUT_Y, int *OUT_Z){

	//syncro();
	//while(!DataRdyUART1());
	WriteUART1('A');
	while(BusyUART1());
        WriteUART1('B');
	while(BusyUART1());
	write_entero((int)*OUT_Z);
	write_entero((int)*OUT_Y);
	write_entero((int)*OUT_X);
        WriteUART1(0X0A);
	while(BusyUART1());
        WriteUART1(0X0D);
	while(BusyUART1());
}

///*********************************************************************
//* Function:         void ConsolePutROMString(ROM char* str)
//*
//* PreCondition:     none
//*
//* Input:		    str - ROM string that needs to be printed
//*
//* Output:		    none
//*
//* Side Effects:	    str is printed to the console
//*
//* Overview:		    This function will print the inputed ROM string
//*
//* Note:			    Do not power down the microcontroller until 
//*                   the transmission is complete or the last 
//*                   transmission of the string can be corrupted.  
//********************************************************************/
//void ConsolePutROMString(ROM char* str)
//{
//    BYTE c;
//
//    while( (c = *str++) )
//        ConsolePut(c);
//}
//
///*********************************************************************
//* Function:         void ConsolePut(BYTE c)
//*
//* PreCondition:     none
//*
//* Input:		    c - character to be printed
//*
//* Output:		    none
//*
//* Side Effects:	    c is printed to the console
//*
//* Overview:		    This function will print the inputed character
//*
//* Note:			    Do not power down the microcontroller until 
//*                   the transmission is complete or the last 
//*                   transmission of the string can be corrupted.  
//********************************************************************/
//void ConsolePut(BYTE c)
//{
//    while(U2STAbits.TRMT == 0);
//    U2TXREG = c;
//}
//
///*********************************************************************
//* Function:         BYTE ConsoleGet(void)
//*
//* PreCondition:     none
//*
//* Input:		    none
//*
//* Output:		    one byte received by UART
//*
//* Side Effects:	    none
//*
//* Overview:		    This function will receive one byte from UART
//*
//* Note:			    Do not power down the microcontroller until 
//*                   the transmission is complete or the last 
//*                   transmission of the string can be corrupted.  
//********************************************************************/
//BYTE ConsoleGet(void)
//{
//	char Temp;
//
//    while(IFS1bits.U2RXIF == 0);
//
//	Temp = U2RXREG;
//    IFS1bits.U2RXIF = 0;
//	return Temp;
//}
//	
//
//
///*********************************************************************
//* Function:         void PrintChar(BYTE toPrint)
//*
//* PreCondition:     none
//*
//* Input:		    toPrint - character to be printed
//*
//* Output:		    none
//*
//* Side Effects:	    toPrint is printed to the console
//*
//* Overview:		    This function will print the inputed BYTE to 
//*                   the console in hexidecimal form
//*
//* Note:			    Do not power down the microcontroller until 
//*                   the transmission is complete or the last 
//*                   transmission of the string can be corrupted.  
//********************************************************************/
//void PrintChar(BYTE toPrint)
//{
//    BYTE PRINT_VAR;
//    PRINT_VAR = toPrint;
//    toPrint = (toPrint>>4)&0x0F;
//    ConsolePut(CharacterArray[toPrint]);
//    toPrint = (PRINT_VAR)&0x0F;
//    ConsolePut(CharacterArray[toPrint]);
//    return;
//}
//
///*********************************************************************
//* Function:         void PrintDec(BYTE toPrint)
//*
//* PreCondition:     none
//*
//* Input:		    toPrint - character to be printed. Range is 0-99
//*
//* Output:		    none
//*
//* Side Effects:	    toPrint is printed to the console in decimal
//*                   
//*
//* Overview:		    This function will print the inputed BYTE to 
//*                   the console in decimal form
//*
//* Note:			    Do not power down the microcontroller until 
//*                   the transmission is complete or the last 
//*                   transmission of the string can be corrupted.  
//********************************************************************/
//void PrintDec(BYTE toPrint)
//{
//    ConsolePut(CharacterArray[toPrint/10]);
//    ConsolePut(CharacterArray[toPrint%10]);
//}
