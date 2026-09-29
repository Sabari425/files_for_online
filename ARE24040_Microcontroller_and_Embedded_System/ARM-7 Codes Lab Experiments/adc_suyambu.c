#include<LPC214x.h>
#include<stdio.h>
#define RS 0x10000
#define RW 0x20000
#define EN 0x40000
#define r1 (1<<24)
#define r2 (1<<25)
#define r3 (1<<26)
#define r4 (1<<27)
#define c1 (1<<28)
#define c2 (1<<29)
#define c3 (1<<30)
#define c4 (1<<31)
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_initialize(void);
void lcd_display(void);
void LCD4_Convert(unsigned char);
void ADC_Init(void);
unsigned int Read_ADC(void);
const unsigned char cmd[4]={0x28,0x0c,0x06,0x01};
unsigned char msg[]={" Dumthalaka "};
unsigned char msg1[]={"~~~~~~~~~~~~~~~~"};
void delay(unsigned int n)
{
 unsigned int i,j;
 for(i=0;i<n;i++)
 {
  for(j=0;j<12000;j++);}
}
void lcd_cmd(unsigned char data)
{
 IOCLR0|=RS;
 IOCLR0|=RW;
 LCD4_Convert(data);
}
void lcd_initialize(void)
{
 int i;
 for(i=0;i<4;i++)
 {
  IOCLR0=0xF<<19;
  lcd_cmd(cmd[i]);
  delay(15);
 }
}
void lcd_data(unsigned char data)
{
 IOSET0|=RS;
 IOCLR0|=RW;
 LCD4_Convert(data);
}
void LCD4_Convert(unsigned char c)
{if(c & 0x80) IOSET0=1<<22; else IOCLR0=1<<22;
 if(c & 0x40) IOSET0=1<<21; else IOCLR0=1<<21;
 if(c & 0x20) IOSET0=1<<20; else IOCLR0=1<<20;
 if(c & 0x10) IOSET0=1<<19; else IOCLR0=1<<19;
 IOSET0=EN;
 delay(8);
 IOCLR0=EN;
 if(c & 0x08) IOSET0=1<<22; else IOCLR0=1<<22;
 if(c & 0x04) IOSET0=1<<21; else IOCLR0=1<<21;
 if(c & 0x02) IOSET0=1<<20; else IOCLR0=1<<20;
 if(c & 0x01) IOSET0=1<<19; else IOCLR0=1<<19;
 IOSET0=EN;
 delay(8);
 IOCLR0=EN;
}
void lcd_display(void)
{
 char i;
 lcd_cmd(0x80);
 delay(15);
 i=0;
 while(msg[i]!='\0')
{
 delay(5);
 lcd_data(msg[i]);
 i++;
 }
 delay(15);
 lcd_cmd(0xc0);
 delay(15);
 i=0;
 while(msg1[i]!='\0')
 {
  delay(5);
  lcd_data(msg1[i]);
  i++;
  }
  delay(15);
}
void ADC_Init()
{
 PINSEL1|=0x01000000;
 AD0CR=0x00200F02;
}
unsigned int Read_ADC()
{
 unsigned int adc_data;
 AD0CR|=0x01000000;
 do{
    adc_data=AD0DR1;
	}while(!(adc_data & 0x80000000));
	adc_data=adc_data>>6;
	adc_data=adc_data & 0x3FF;
	return(adc_data);
}
 int main(void)
 {
  unsigned int adc_data;
  unsigned char ones,tens,hundreds,thousands;
  PINSEL0=0x00000000;
  IODIR0=0xffffffff;
  PINSEL1=0x00000000;
  IODIR1=0xf0000000;
  lcd_initialize();
  lcd_display();
  ADC_Init();
  delay(1000);
  while(1)
  {
   adc_data=Read_ADC();
   ones=adc_data % 10;
   adc_data=adc_data / 10;
   tens=adc_data % 10;
   adc_data=adc_data / 10;
   hundreds=adc_data % 10;
   adc_data=adc_data / 10;
   thousands=adc_data % 10;
   lcd_cmd(0xC0);
   lcd_data(thousands|0x30);
   lcd_data(hundreds|0x30);
   lcd_data(tens|0x30);
   lcd_data(ones|0x30);
   delay(100000);
   delay(100000);
   delay(100000);
   delay(100000);
   }
}