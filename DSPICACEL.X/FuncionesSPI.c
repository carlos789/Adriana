#include <p33EP128MC202.h>
#include <spi.h>


void ConfiguracionSPI(void);
void Write_SPI ( short command );
void Delay5mseg ( void );
void Delay ( void );
void EscrituraON (void);
void EscribirStatus (int valor);
void LeerStatus (int* dato1);
//void EscribirPagina (int f, int g, int h);
void EscribirPagina (int f, int g, int h, int* v[256]);
void LeerPagina (int m, int n,int o);
unsigned int SPIRead (void);
void ReadIdentification (int* a,int* b,int* c);
void ReadRES (int* d);
void Delay05mseg ( void );
void ReadSecurityRegister(int* j);
void SectorErase(void);
void ChipErase(void);
void EscribirPaginaIndex (int f, int g, int h,int r, int* v[256]);
void EscribirDato (int* dato);
unsigned int LeerDato (void);

#define CS LATBbits.LATB12


void ConfiguracionSPI(void)
{
    IFS2bits.SPI2IF = 0;      // Clear the Interrupt flag
    IEC2bits.SPI2IE = 0;      // Disable the interrupt

    // SPI1CON1 Register Settings
    SPI2CON1bits.DISSCK = 0;    // Internal serial clock is enabled.
    SPI2CON1bits.DISSDO = 0;    // SDOx pin is controlled by the module.
    SPI2CON1bits.MODE16 = 0;    // Communication is byte-wide (8 bits).
    SPI2CON1bits.MSTEN = 1;     // Master mode enabled.
    SPI2CON1bits.SMP = 0;       // Input data is sampled at the middle of data output time.
    SPI2CON1bits.CKE =1;        // Serial output data changes on transition from.

    SPI2CON2bits.SPIBEN = 0;

    //SPI Clock frequency = Fcy/(Secondary prescaler * Primary prescaler) = 60MHz/(1*4) = 15MHz (MAX).
    SPI2CON1bits.PPRE = 0b01;   //Primary prescaler.
    SPI2CON1bits.SPRE = 0b111;  //Secondary prescaler.

    // Idle clock state to active clock state
    SPI2CON1bits.CKP =0; // Idle state for clock is a low level; cambie.....era 0

    // active state is a high level
    SPI2STATbits.SPIEN = 1; // Enable SPI module
}

void Write_SPI ( short command )
{
    int temp;

    temp = SPI2BUF;                             // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI2BUF = command & 0xFF;                   // write the data out to the SPI peripheral
    while( !SPI2STATbits.SPIRBF );              // wait for the data to be sent out
}


void Delay5mseg ( void ) //Aprox
{
    int temp;
    for( temp = 0; temp < 7500; temp++ ); //1ms
    for( temp = 0; temp < 7500; temp++ ); //1ms
    for( temp = 0; temp < 7500; temp++ ); //1ms
    for( temp = 0; temp < 7500; temp++ ); //1ms
    for( temp = 0; temp < 7500; temp++ ); //1ms
}

void Delay1mseg ( void )
{
    int temp = 0x00;
    for( temp = 0; temp < 7500; temp++ ); //1ms

}

void Delay05mseg ( void )
{
    int temp = 0x00;
    for( temp = 0; temp < 3750; temp++ ); //0.5ms

}
void EscrituraON (void)
{
    CS = 0;
    Write_SPI(0x06);    //habilito escritura 0x06
    CS = 1;
}

void EscribirStatus (int valor)
{
    int temp = 0;

    EscrituraON();

    CS = 0;
    Write_SPI(0x01);    //voy a escribir el registro status
    Write_SPI(valor);    //valor a escribir
    CS = 1;

    while(temp<8)//delay 40ms
    {
        Delay5mseg();
        temp++;
    }
}

void LeerStatus (int* dato1)
{
   CS = 0;
   Write_SPI(0x05);    //voy a leer el status

   *dato1 = SPIRead();

   CS = 1;
}

void EscribirPagina (int f, int g, int h, int* v[256])
{
    int i=0x00;
    int status = 0x00;

    EscrituraON();

    while(status!=0x02)
    {
        LeerStatus(&status);
    }

    CS = 0;

    Write_SPI(0x02);    //programar pagina.
 
    Write_SPI(f);    //dirección
    Write_SPI(g);
    Write_SPI(h);

    while(i<256)
    {
        //Write_SPI(i);
        Write_SPI(v[i]);
        i++;
    }

    CS = 1;
}

void LeerPagina (int m, int n,int o)
{

    int i=0x00;
    int aux = 0x00;
    CS = 0;

    Write_SPI(0x03);    //leer.

    Write_SPI(m);    //dirección
    Write_SPI(n);
    Write_SPI(o);

    while(i<256)
    {
        aux = SPIRead();
        WriteUART1(aux);
        while(BusyUART1());
        Delay05mseg();
        i++;
    }

    CS = 1;
}

unsigned int SPIRead (void)
{
    //unsigned int DatoSPI = 0x0000;
    //SPI2BUF = 0xFF;               //Transmit a dummy word
    //while (!SPI2STATbits.SPIRBF);   //wait until the end of reception
    //DatoSPI = SPI2BUF;                  //Load buffer content into the returned variable
    //return (DatoSPI);
    
    unsigned int tmp = 0;
    Write_SPI(0x00);
    while(!DataRdySPI2());
    tmp = SPI2BUF;
    return tmp;
}


void ReadIdentification (int* a,int* b,int* c)
{
    CS = 0;

    Write_SPI(0x9F);
    *a = SPIRead();
    *b = SPIRead();
    *c = SPIRead();

    CS = 1;
}

void ReadRES (int* d)
{
    CS = 0;

    Write_SPI(0xAB);
    Write_SPI(0x00);
    Write_SPI(0x00);
    Write_SPI(0x00);

    *d = SPIRead();

    CS = 1;
}

void ReadSecurityRegister(int* j)
{
    CS = 0;

    Write_SPI(0x2B);
    *j = SPIRead();

    CS = 1;
}

void SectorErase(void)
{
    int temp = 0;
    EscrituraON();

    CS = 0;

    Write_SPI(0x20);    //Erase Sector

    Write_SPI(0x1F);    //Direccion
    Write_SPI(0xFE);
    Write_SPI(0xFF);

    CS = 1;

    while(temp<20)//delay 20s
    {
        Delay5mseg();
        temp++;
    }
}

void ChipErase(void)
{
    int temp = 0;

    EscrituraON();

    CS = 0;

    Write_SPI(0x60);

    CS = 1;

    while(temp<4000)//delay 20s
    {
        Delay5mseg();
        temp++;
    }
}

void EscribirPaginaIndex (int f, int g, int h,int r, int* v[256])
{
    int i=r;
    int status = 0x00;

    EscrituraON();

    while(status!=0x02)
    {
        LeerStatus(&status);
    }

    CS = 0;

    Write_SPI(0x02);    //programar pagina.

    Write_SPI(f);    //dirección
    Write_SPI(g);
    Write_SPI(h);

    while(i<252)//valores viejos
    {
        Write_SPI(v[i]);
        i++;
    }
    i=0x00;
    while(i<r)//valores recientes
    {
        Write_SPI(v[i]);
        i++;
    }
    i=252;//tenia 253
    while(i<256) //pagina
    {
        Write_SPI(v[i]);
        i++;
    }

    CS = 1;
}


void EscribirDato (int* dato)
{

    int status = 0x00;

    EscrituraON();

    while(status!=0x02)
    {
        LeerStatus(&status);
    }

    CS = 0;

    Write_SPI(0x02);    //programar pagina.

    Write_SPI(0x1F);    //última pagina
    Write_SPI(0xFE);
    Write_SPI(0xFF);

    Write_SPI(dato);

    CS = 1;
    Delay05mseg();
}

unsigned int LeerDato (void)
{

    int i=0x00;
    int aux = 0x00;
    CS = 0;

    Write_SPI(0x03);    //leer.

    Write_SPI(0x1F);    //dirección
    Write_SPI(0xFE);
    Write_SPI(0xFF);

    aux = SPIRead();

    CS = 1;
    Delay05mseg();
    return aux;

}