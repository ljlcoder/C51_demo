#include <reg52.h>

sfr WDT_CONTR=0xe1; //?????????
sbit led=P2^7;

void delayms(unsigned int xms) {
    unsigned int i,j;
    for (i=xms;i>0;i--)
    for(j=110;j>0;j--);
}

void main()
{
    WDT_CONTR=0x35;  //?????,??????,?????64,2s?????????
    led=0;
    delayms(500);
    led=1;
    while(1)
    {
        delayms(3000);
        WDT_CONTR=0x35;
    }
}
