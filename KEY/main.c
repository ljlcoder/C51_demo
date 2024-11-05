#include <reg52.h>
#include "intrins.h"
sbit led=P2^7;
sbit key=P3^7;

void delay_ms(unsigned int xms)   //@12MHz
{
    unsigned int i, j;
    for(i=xms;i>0;i--)
    {
        for(j=124;j>0;j--)
        {}
    }
}

void main(){
	while(1){
		delay_ms(10);
		if(key==0)
		{
			  led=~led;
				delay_ms(1000);
		}
	}
}