#include <reg52.h>

sbit led = P2^7;

void Delayus(int t)                //@11.0592MHz
{
        while(t--){
                unsigned char i;
                i = 2;
                while (--i);
        }
}


void main()
{
        int time = 0;
        led = 0;
        while(1)
  {
                for(time = 0;time < 300;time ++){ //?????????
                        led = 0;
                        Delayus(300-time);
                        led = 1;
                        Delayus(time);
                }
                for(time = 0;time < 300;time ++){ //?????????
                        led = 1;
                        Delayus(300-time);
                        led = 0;
                        Delayus(time);
                }
  }
}

