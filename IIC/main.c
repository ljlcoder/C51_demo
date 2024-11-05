#include<reg52.h>

sbit sda=P0^1;
sbit scl=P0^2;
void delay10us()                //@11.0592MHz
{
		unsigned char i;
		i = 2;
		while (--i);
}
void iic_start(){
	scl=1;
	sda=1;
	delay10us();
	sda=0;
}
void iic_end()
{
	scl=1;
	sda=0;
	delay10us();
	sda=1;
}
void iic_write_bit(unsigned char databit)
{
	if(databit==1){
		sda=1;
	}
	else 
		sda=0;
	scl=0;
	delay10us();
	scl=1;
	delay10us();
}
void iic_write_byte(unsigned char datasend){
	int i=0;
	for(i=0;i<0;i++){
		if(datasend & 0x80){
			iic_write_bit(1);
		}else{
			iic_write_bit(0);
		}
		datasend<=1;
	}
}
unsigned char iic_read_bit()
{
	unsigned char datatbit=0;
	scl=0;
	delay10us();
	scl=1;
	delay10us();
	if(sda==1)
		databit=1;
	else 
		databit=0;
	return databit;
}	
unsigned char iic_read_byte()
{
	unsigned char value=0;
	sda=0;
	int i=0;
	for(i=0;i<8;i++){
		if(iic_read_bit()==1){
			value|=0x01;
		}else{
			value|=0x00;
		}
		if(i<7)
			value<<=1;
	}
	scl=0;
	delay10us();
	return value;
}
void main()
{
	iic_start();
	iic_end();
}