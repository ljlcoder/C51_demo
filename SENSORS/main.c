#include <reg52.h>
#include <stdio.h> 

sbit sda = P0^1;
sbit scl = P0^2;

#define GXHT3L_ADDR_WRITE 0x44<<1    //0b0100 0100 -> 0b1000 1000
#define GXHT3L_ADDR_READ  (0x44<<1)+1//0b0100 0100 -> 0b1000 1001

void Delay10us()                //@11.0592MHz
{
        unsigned char i;
        i = 2;
        while (--i);
}

void i2c_start()
{
        scl = 1;
        sda = 1;
        Delay10us();
        sda = 0;
}

void i2c_stop()
{
        scl = 1;
        sda = 0;
        Delay10us();
        sda = 1;
}

void i2c_write_bit(unsigned char databit)
{
        scl = 0;
        if(databit == 1)
                sda = 1;
        else
                sda = 0;
        Delay10us();
        scl = 1;
        Delay10us();
}

void i2c_write_byte(unsigned char datasend)
{
        int i = 0;
        for(i = 0;i< 8;i++){
                if(datasend & 0x80){
                        i2c_write_bit(1);
                }else{
                        i2c_write_bit(0);
                }
                datasend = datasend << 1;
        }
}

unsigned char i2c_read_bit()
{
        unsigned char databit = 0;
        scl = 0;
        Delay10us();
        scl = 1;
        Delay10us();
        if(sda == 1)
                databit =  1;
        else
                databit = 0;
        return databit;
}

unsigned char i2c_read_byte()
{
        unsigned char value = 0;
        unsigned char i = 0;
        sda = 1; //?????????
        for(i = 0;i < 8;i++){
                if(i2c_read_bit() == 1){
                        value = value | 0x01;
                }else{
                        value = value | 0x00;
                }
                if(i<7)
                        value = value << 1;
        }
        scl = 0;
        Delay10us();
        return value;
}

void i2c_ack()
{
        scl = 0;
        sda = 0;
        Delay10us();
        scl = 1;
        Delay10us();
        scl = 0;
}

void i2c_nack()
{
  scl = 0;
  sda = 1;     //SDA??,???????
  Delay10us();
  scl = 1;
  Delay10us(); 
  scl = 0;
}

void i2c_wait_ack()
{
        unsigned char time = 0;
        scl = 0;
        sda = 1;
        Delay10us();
        scl = 1;
        Delay10us();
        while(sda){
                Delay10us();
                time ++;
                if(time > 100)
                        break;
        }
        scl = 0;
        Delay10us();
}

void gxht30_init()
{
        i2c_start();
        i2c_write_byte(GXHT3L_ADDR_WRITE);
        i2c_wait_ack();
        i2c_write_byte(0x22);
        i2c_wait_ack();
        i2c_write_byte(0x20);
        i2c_wait_ack();
                                i2c_stop();
}

void gxht30_read_mode()
{
        i2c_start();
        i2c_write_byte(GXHT3L_ADDR_WRITE);
        i2c_wait_ack();
        i2c_write_byte(0xE0);
        i2c_wait_ack();
        i2c_write_byte(0x00);
        i2c_wait_ack();
                                i2c_stop();
                                Delay10us();
}

#define POLYNOMIAL  0x31   // P(x) = x^8 + x^5 + x^4 + 1 = 00110001
 
//CRC????
unsigned char gxht30_crc8(unsigned char *crcdata, unsigned char nbrOfBytes)
{
  unsigned char Bit;        // bit mask
  unsigned char crc = 0xFF; // calculated checksum
  unsigned char byteCtr;    // byte counter
  for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
  {
    crc ^= (crcdata[byteCtr]);
    for(Bit = 8; Bit > 0; --Bit)
    {
      if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else           crc = (crc << 1);
    }
  }
  return crc;
}

void gxht30_read_data()
{
        unsigned short tem,hum;
        int index = 0;
        float temperature,humidity;
        unsigned char buffer[6];
        i2c_start();
        i2c_write_byte(GXHT3L_ADDR_READ);
        i2c_wait_ack();
        for(index = 0; index < 6;index ++){
                buffer[index] = i2c_read_byte();
                if(index == 5)
                        i2c_nack();
                else
                        i2c_ack();
        }
        i2c_stop();
                                if(gxht30_crc8(buffer,2) != buffer[2]){
                                        printf("crc error\n");
                                        return;
                                }
                                if(gxht30_crc8(&buffer[3],2) != buffer[5]){
                                        printf("crc error\n");
                                        return;
                                }
        //????8bit??????16bit???
        tem = (buffer[0] << 8) | buffer[1];
        hum = (buffer[3] << 8) | buffer[4];
        //???????
        temperature = (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
                                humidity = (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1) 
        printf("temperature=%f humidity=%f\n",temperature,humidity);
}


void uart_init(void)                //9600bps@11.0592MHz
{
        PCON &= 0x7F;                //??????
        SCON = 0x50;                //8???,?????
        TMOD &= 0x0F;                //?????1???
        TMOD |= 0x20;                //?????1?8???????
        TL1 = 0xFD;                //??????
        TH1 = 0xFD;                //????????
        ET1 = 0;                //?????1??
        TR1 = 1;                //?????1
}

/*
**??printf???putchar??,????????
**???????<stdio.h>
*****/
char putchar(char dat){
        //????????
        SBUF = dat;     //?????????
        while(!TI);    //??????,TI??????? ?1
        TI = 0;      //????????
        return dat;  //?????????printf
}

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
        uart_init();
        gxht30_init();
        while(1)
        {
                                                                delay_ms(1000);
                gxht30_read_mode();
                gxht30_read_data();
                
        }
}