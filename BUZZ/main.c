#include<reg52.h>

sbit speak=P2^1;

//??????
void delay_ms(unsigned int xms)   //@12MHz
{
    unsigned int i, j;
    for(i=xms;i>0;i--)
    {
        for(j=124;j>0;j--)
        {}
    }
}

void main()
{
   while(1){
   speak=1;
   delay_ms(1);
   speak=0;
   delay_ms(1); 
   }
}