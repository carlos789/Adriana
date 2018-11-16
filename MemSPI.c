#include <p33EP512GM304.h>
#include "MemSPI.h"
#include "MSPI1.h"
#define FCY 60000000UL//
#include <libpic30.h>

void EscrituraON(void) {
    CS_M = 0;
    WByteSPI2(0x06); //habilito escritura 0x06
    CS_M = 1;
}

void EscribirStatus(unsigned char valor) {
    unsigned char temp = 0;
    EscrituraON();
    temp = valor;
    CS_M = 0;
    WByteSPI2(0x01); //RDSR voy a escribir el registro status
    WByteSPI2(temp); //valor a escribir
    CS_M = 1;

    //    while(temp<8)//delay 40ms
    //    {
    //        Delay5mseg();
    //        temp++;
    //    }
}

unsigned char LeerStatus(void) {
    unsigned char dato, basura;
    CS_M = 0;
    WByteSPI2(0x05); //voy a leer el status
    dato = RByteSPI2();
    basura = RByteSPI2();
    CS_M = 1;
    return dato;
}

unsigned char LeerStatus1(int* a) {
    CS_M = 0;
    WByteSPI2(0x05); //voy a leer el status
    *a = RByteSPI2();
    //   dato = RByteSPI2();
    //dato = RByteSPI2();
    CS_M = 1;
    return 0;
}

unsigned char LeerConf1(int* a) {
    CS_M = 0;
    WByteSPI2(0x05); //voy a leer el status
    a = RByteSPI2();
    //   dato = RByteSPI2();
    //dato = RByteSPI2();
    CS_M = 1;
    return 0;
}

void UnlockMem(void) //Global Block Protection Unlock
{
    unsigned char estatus;
    EscrituraON();
    while (estatus == 0x01) {
        estatus = LeerStatus();
        estatus = estatus & 0x01;
    }
    CS_M = 0;
    WByteSPI2(0x98); //envio comando para Unlock
    CS_M = 1;
}

void EscribirPagina(int f, int g, int h, unsigned char* v[256]) {
    int i = 0x00;
    int estatus = 0x01;

    EscrituraON();

    while (estatus == 0x01) {
        estatus = LeerStatus() &0x01;
    }

    CS_M = 0;

    WByteSPI2(0x02); //programar pagina.
    WByteSPI2(f); //dirección
    WByteSPI2(g);
    WByteSPI2(h);

    while (i < 256) {
        WByteSPI2(VectorMEM[i]);
        i++;
    }

    CS_M = 1;

}

void LeerPagina(int m, int n, int o, unsigned char *v[256]) //m memoria, n direccion, 
{
    int estatus = 0x01;
    while (estatus == 0x01) {
        estatus = LeerStatus() &0x01;
    }

    int i = 0x01;
    int aux = 0x00;
    CS_M = 0;

    WByteSPI2(0x03); //leer.

    WByteSPI2(m); //dirección
    WByteSPI2(n);
    WByteSPI2(o);

    while (i < 256) {
        VectorMEM[i] = RByteSPI2();
        i++;
    }

    CS_M = 1;
}

void ReadIdentification(int* a, int* b, int* c) {
    //    unsigned char ff,uno,dos,tres;
    CS_M = 0;
    WByteSPI2(0x9F);
    //    ff= RByteSPI2();//Gran duda
    *a = RByteSPI2();
    *b = RByteSPI2();
    *c = RByteSPI2();
    //    uno = RByteSPI2();
    //    dos = RByteSPI2();
    //    tres = RByteSPI2();
    CS_M = 1;
}

void SectorErase(void) {
    int temp = 0;
    EscrituraON();

    CS_M = 0;

    WByteSPI1(0x20); //Erase Sector

    WByteSPI1(0x1F); //Direccion
    WByteSPI1(0xFE);
    WByteSPI1(0xFF);

    CS_M = 1;

    while (temp < 20)//delay 20s
    {
        Delay5mseg();
        temp++;
    }
}

void ChipErase(void)//Borro toda la memoris
{
    LeerPagina(0x7f, 0xfe, 0, VectorMEM); //leo para guargar el numero de pipe
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
    EscribirPagina(0x7f, 0xfe, 0, VectorMEM); //volvemos a escribir para guardar el numero de pipe
}

void EscribirPaginaIndex(int f, int g, int h, int r, int* v[256])//No se usa
{
    int i = r;
    int estatus = 0x00;

    EscrituraON();

    while (estatus != 0x01) {
        estatus = LeerStatus()&0x01;
    }

    CS_M = 0;

    WByteSPI1(0x02); //programar pagina.

    WByteSPI1(f); //dirección
    WByteSPI1(g);
    WByteSPI1(h);

    while (i < 252)//valores viejos
    {
        WByteSPI1(v[i]);
        i++;
    }
    i = 0x00;
    while (i < r)//valores recientes
    {
        WByteSPI1(v[i]);
        i++;
    }
    i = 252; //tenia 253
    while (i < 256) //pagina
    {
        WByteSPI1(v[i]);
        i++;
    }

    CS_M = 1;
}

void EscribirDato(unsigned char datoHH, unsigned char datoH, unsigned char datoL, unsigned char DATO) {

    int estatus = 0x00;

    EscrituraON();
    while (estatus == 0x01) //veo si el  estatus es distinto de 0x01
    {
        estatus = LeerStatus()&0x01;
    }

    CS_M = 0;
    WByteSPI2(0x02); //escribir
    WByteSPI2(datoHH); //dirección mas significativa
    WByteSPI2(datoH);
    WByteSPI2(datoL); //dirección menos significativa
    WByteSPI2(DATO);
    CS_M = 1;

}

unsigned char LeerDato(unsigned char datoHH, unsigned char datoH, unsigned char datoL) {
    int aux = 0x00;
    CS_M = 0;
    WByteSPI2(0x03); //leer.
    WByteSPI2(datoHH); //dirección mas significativa
    WByteSPI2(datoH);
    WByteSPI2(datoL); //dirección menos significativa
    //aux = RByteSPI2();// Retorna el dato de la memoria
    aux = RByteSPI2(); // Retorna el dato de la memoria
    CS_M = 1;
    return aux;

}

void Delay5mseg(void) //Aprox
{
    int temp;
    for (temp = 0; temp < 7500; temp++); //1ms
    for (temp = 0; temp < 7500; temp++); //1ms
    for (temp = 0; temp < 7500; temp++); //1ms
    for (temp = 0; temp < 7500; temp++); //1ms
    for (temp = 0; temp < 7500; temp++); //1ms
}

void Delay1mseg(void) {
    int temp = 0x00;
    for (temp = 0; temp < 7500; temp++); //1ms

}

void Delay05mseg(void) {
    int temp = 0x00;
    for (temp = 0; temp < 3750; temp++); //0.5ms

}