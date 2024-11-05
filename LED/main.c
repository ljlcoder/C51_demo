#include "reg52.h"
#include "intrins.h"

sbit LED1 = P2^7;
sbit LED2 = P2^6;
sbit LED3 = P2^5;

void Delay1000ms()                //@11.0592MHz
{
        unsigned char i, j, k;

        _nop_();
        i = 8;
        j = 1;
        k = 243;
        do
        {
                do
                {
                        while (--k);
                } while (--j);
        } while (--i);
}

void main(){
        while(1){
                LED1 = 0;
                LED2 = 1;
                LED3 = 1;
                Delay1000ms();
                LED2 = 0;
                LED1 = 1;
                LED3 = 1;
                Delay1000ms();
                LED3 = 0;
                LED1 = 1;
                LED2 = 1;
                Delay1000ms();
        }
}