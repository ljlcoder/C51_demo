#include <reg52.h>

sbit ds_pin = P0^3;
sbit stcp_pin = P0^4;
sbit shcp_pin = P0^5;

//?? ?????:0-9
unsigned char num[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void hc595_send_byte(unsigned char byte)
{
        unsigned int i;
        for(i = 0; i < 8; i++)
        {
                //??????,??????????????????????????
                if(byte & 0x80)
                        ds_pin = 1;
                else
                        ds_pin = 0;
                //SHCP??????????,74HC595???DS??????????
                shcp_pin = 0;
                shcp_pin = 1;
                byte <<= 1;
        }
}

//num-??????? addr-????????
void hc595_send_data(unsigned char num, unsigned char addr)
{
        hc595_send_byte(num); //?????????
        //?????????8?????,???????????,?????9????
        //??????????,???????????????????595?
        if(addr == 0)
            hc595_send_byte(0xFE);  //Q0??  0b1111 1110 0xFE 
        else if(addr == 1)    
            hc595_send_byte(0xFD);  //Q1??  0b1111 1101 0xFD 
        //???????8??????????,???????????????(??????????)
        stcp_pin = 0;
        stcp_pin = 1;
}

void main(){
  while(1)
  {
    //???????????0
    hc595_send_data(num[0], 0);
    //???????????3
    hc595_send_data(num[3], 1);
  }
}
