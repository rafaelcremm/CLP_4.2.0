#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define _XTAL_FREQ 20000000

#define RS LATBbits.LATB4
#define EN LATBbits.LATB5
#define RW LATBbits.LATB6
#define D4 LATBbits.LATB0
#define D5 LATBbits.LATB1
#define D6 LATBbits.LATB2
#define D7 LATBbits.LATB3

#define T_RS TRISBbits.TRISB4
#define T_EN TRISBbits.TRISB5
#define T_RW TRISBbits.TRISB6
#define T_D4 TRISBbits.TRISB0
#define T_D5 TRISBbits.TRISB1
#define T_D6 TRISBbits.TRISB2
#define T_D7 TRISBbits.TRISB3

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(const char * a);
void Lcd_White_Center_String(char linha, const char * a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Inicia_LCD(void);
void escreve_LCD(char linha, char coluna,const char * str );
void erro_lcd(char linha, char coluna, char erro);

//
//void Lcd_Port(char a)
//{
//   if(a & 1)
//      D4 = 1;
//   else
//      D4 = 0;
//
//   if(a & 2)
//      D5 = 1;
//   else
//      D5 = 0;
//
//   if(a & 4)
//      D6 = 1;
//   else
//      D6 = 0;
//
//   if(a & 8)
//      D7 = 1;
//   else
//      D7 = 0;
//}
//void Lcd_Cmd(char a)
//{
//   RS = 0;             // => RS = 0
//   Lcd_Port(a);
//   EN  = 1;             // => E = 1
//   delay(4);
//   EN  = 0;             // => E = 0
//}
//
//void Lcd_Clear(void)
//{
//   Lcd_Cmd(0);
//   Lcd_Cmd(1);
//}
//
//void Lcd_Set_Cursor(char a, char b)
//{
//   char temp,z,y;
//   if(a == 1)
//   {
//     temp = 0x80 + b - 1;
//      z = temp>>4;
//      y = temp & 0x0F;
//      Lcd_Cmd(z);
//      Lcd_Cmd(y);
//   }
//   else if(a == 2)
//   {
//      temp = 0xC0 + b - 1;
//      z = temp>>4;
//      y = temp & 0x0F;
//      Lcd_Cmd(z);
//      Lcd_Cmd(y);
//   }
//}
//
//void Lcd_Init(void)
//{
//    T_RS = 0;
//    T_EN = 0;
//    T_D4 = 0;
//    T_D5 = 0;
//    T_D6 = 0;
//    T_D7 = 0;
//  Lcd_Port(0x00);
//   delay(20);
//  Lcd_Cmd(0x03);
//   delay(5);
//  Lcd_Cmd(0x03);
//   delay(11);
//  Lcd_Cmd(0x03);
//  /////////////////////////////////////////////////////
//  Lcd_Cmd(0x02);
//  Lcd_Cmd(0x02);
//  Lcd_Cmd(0x08);
//  Lcd_Cmd(0x00);
//  Lcd_Cmd(0x0C);
//  Lcd_Cmd(0x00);
//  Lcd_Cmd(0x06);
//}
//
//void Lcd_Write_Char(char a)
//{
//   char temp,y;
//   temp = a&0x0F;
//   y = a&0xF0;
//   RS = 1;             // => RS = 1
//   Lcd_Port(y>>4);             //Data transfer
//   EN = 1;
//   __delay_us(40);
//   EN = 0;
//   Lcd_Port(temp);
//   EN = 1;
//   __delay_us(40);
//   EN = 0;
//}
//
//void Lcd_Write_String(const char * a)
//{
//   int i;
//   for(i=0;a[i]!='\0';i++)
//      Lcd_Write_Char(a[i]);
//}
//void Lcd_White_Center_String(char linha, const char * a){
//    char size,coluna;
//    size = strlen(a);
//    if(size>16){size=16;}
//    coluna = ((16-size)/2)+1;
//    Lcd_Set_Cursor(linha,coluna);
//    int i;
//    for(i=0;a[i]!='\0';i++)
//    Lcd_Write_Char(a[i]);
//}
//void Lcd_Shift_Right(void)
//{
//   Lcd_Cmd(0x01);
//   Lcd_Cmd(0x0C);
//}
//
//void Lcd_Shift_Left(void)
//{
//   Lcd_Cmd(0x01);
//   Lcd_Cmd(0x08);
//}
//void Inicia_LCD(void){
//    unsigned int a;
//    Lcd_Init();
//    Lcd_Clear();
//    Lcd_Set_Cursor(1,5);
//    Lcd_Write_String("SONITRON");
//    Lcd_Set_Cursor(2,1);
//    Lcd_Write_String("INICIALIZANDO...");
//    delay(700);
////    Lcd_Clear();
////    Lcd_Set_Cursor(1,1);
////    Lcd_Write_String("Developed By");
////    Lcd_Set_Cursor(2,1);
////    Lcd_Write_String("Rafael Cremm");
////    delay(700);
////    Lcd_Clear();
////    Lcd_Set_Cursor(1,1);
////    Lcd_Write_String("   ANALIZADOR   ");
////    Lcd_Set_Cursor(2,1);
////    Lcd_Write_String("Pronto Para Uso");
////    delay(1000);
////    for(a=0;a<15;a++)
////    {
////        delay(300);
////        Lcd_Shift_Left();
////    }
////    for(a=0;a<15;a++)
////    {
////        delay(300);
////        Lcd_Shift_Right();
////    }
////    delay(1000);
//}

