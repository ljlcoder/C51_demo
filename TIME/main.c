#include<reg52.h>
#define TIMS 65536-9216
sbit led=P2^7;
int count=0;
void time() interrupt 1
{
	 count++;
	if(count >=100)
	{
		count=0;
		led=~led;
	}
	TL0=TIMS;
	TH0=TIMS>>8;
}
void main(){
	TMOD=0x01;
	TR0=1;
	
	TL0=TIMS;
	TH0=TIMS>>8;
	
	ET0=1;
	EA=1;
	
	while(1){};
}