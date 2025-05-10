#include "lcd.h"
#include "eusart.h"
void Lcd_Port(char a)
{
   if(a & 1)
      D4 = 1;
   else
      D4 = 0;

   if(a & 2)
      D5 = 1;
   else
      D5 = 0;

   if(a & 4)
      D6 = 1;
   else
      D6 = 0;

   if(a & 8)
      D7 = 1;
   else
      D7 = 0;
}
void Lcd_Cmd(char a)
{
    RW=0;
   RS = 0;             // => RS = 0
   Lcd_Port(a);
   EN  = 1;             // => E = 1
   __delay_ms(4);
   EN  = 0;             // => E = 0
   RW=1;
}

void Lcd_Clear(void)
{
   RW=0;
   Lcd_Cmd(0);
   Lcd_Cmd(1);
   RW=1;
}

void Lcd_Set_Cursor(char a, char b)
{
    RW=0;
   char temp,z,y;
   if(a == 1)
   {
     temp = 0x80 + b - 1;
      z = temp>>4;
      y = temp & 0x0F;
      Lcd_Cmd(z);
      Lcd_Cmd(y);
   }
   else if(a == 2)
   {
      temp = 0xC0 + b - 1;
      z = temp>>4;
      y = temp & 0x0F;
      Lcd_Cmd(z);
      Lcd_Cmd(y);
   }
   RW=1;
}

void Lcd_Init()
{
    T_RW = 0;
    T_RS = 0;
    T_EN = 0;
    T_D4 = 0;
    T_D5 = 0;
    T_D6 = 0;
    T_D7 = 0;
    RW = 0;
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
   __delay_ms(5);
  Lcd_Cmd(0x03);
   __delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
  RW=1;
}

void Lcd_Write_Char(char a)
{
   RW = 0;
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
   RW=1;
   //EUSART_Write(a);
}

void Lcd_Write_String(const char * a)
{
//   printf("\n\r%s",a);
   RW=0;
   int i;
   for(i=0;a[i]!='\0';i++)
      Lcd_Write_Char(a[i]);
   RW=1;
}
void Lcd_White_Center_String(char linha, const char * a){
    RW=0;
    char size,coluna;
    size = strlen(a);
    if(size>16){size=16;}
    coluna = ((16-size)/2)+1;
    Lcd_Set_Cursor(linha,coluna);
    int i;
    for(i=0;a[i]!='\0';i++)
        Lcd_Write_Char(a[i]);
    RW=1;
//    printf("\n\r");
}
void Lcd_Shift_Right()
{
   RW=0;
   Lcd_Cmd(0x01);
   Lcd_Cmd(0x0C);
   RW=1;
}

void Lcd_Shift_Left()
{
   RW=0;
   Lcd_Cmd(0x01);
   Lcd_Cmd(0x08);
   RW=1;
}
void Inicia_LCD(void){
    RW=0;
//    unsigned int a;
    Lcd_Init();
    Lcd_Clear();
//    escreve_LCD(1,1,inicia_str);
//    escreve_LCD(2,1,aguarde_str);
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String();
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_String(inicia_str);
//    __delay_ms(700);
//    Lcd_Clear();
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String("Developed By");
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_String("Rafael Cremm");
//    __delay_ms(700);
//    Lcd_Clear();
//    Lcd_Set_Cursor(1,1);
//    Lcd_Write_String("   ANALIZADOR   ");
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_String("Pronto Para Uso");
//    __delay_ms(1000);
//    for(a=0;a<15;a++)
////    {
////        __delay_ms(300);
////        Lcd_Shift_Left();
////    }
////    for(a=0;a<15;a++)
////    {
////        __delay_ms(300);
////        Lcd_Shift_Right();
////    }
    RW = 1;
    __delay_ms(1);
}
void escreve_LCD(char linha, char coluna,const char * str ){
    RW=0;
    Lcd_Set_Cursor(linha,coluna);
    Lcd_Write_String(str);
    RW=1;
}

//void erro_lcd(char linha, char coluna, char erro){
//    char e1,e2;
//    RW=0;
//    Lcd_Set_Cursor(linha,coluna);
//    Lcd_Write_String(erro_str);
//    e1=(erro%10) +'0';
//    e2=(erro/10) +'0';
//    Lcd_Write_Char(e2);
//    Lcd_Write_Char(e1);
//    Lcd_Write_String("         ");
//    RW=1;
//}
