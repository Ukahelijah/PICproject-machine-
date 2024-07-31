// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should ` project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#define _XTAL_FREQ 20000000
#define RS RC3
#define EN RC5
#define D4 RD4
#define D5 RD5
#define D6 RD6 
#define D7 RD7
#define StartButton RB1
#define ModeButton RB2
#define Bcheck RB0
#define Mode1 RC0
#define Mode2 RC1
#define Mode3 RC2
#define power RD1
#define pumping RC4
#define filled RD0
#define buzzer RC6
#define pump  RC7

int ChangeMode;




void LCD_SetBit(char data_bit){
    if(data_bit& 1)
        D4=1;
    else
        D4=0;
     if(data_bit& 2)
        D5=1;
    else
        D5=0;
     if(data_bit& 4)
        D6=1;
    else
        D6=0;
     if(data_bit& 8)
        D7=1;
    else
        D7=0;
}
void LCD_Cmd (char a){
   RS=0;
   LCD_SetBit(a);
   EN=1;
   __delay_ms(4);
   EN=0;
   
}
void LCD_Clear(){
    LCD_Cmd (0);
    LCD_Cmd (1);
}
void LCD_Start(){
    LCD_SetBit(0x00);
    for(int i=1065244; i<=0; i--)NOP();
    LCD_Cmd(0x03);
    __delay_ms(5);
     LCD_Cmd(0x03);
    __delay_ms(11);
     LCD_Cmd(0x03);
      LCD_Cmd(0x02);
       LCD_Cmd(0x02);
        LCD_Cmd(0x08);
         LCD_Cmd(0x00);
          LCD_Cmd(0x0C);
           LCD_Cmd(0x00);
            LCD_Cmd(0x06);
            
    
    
}

void LCD_Set_Cursor(char a, char b){
    char temp,z,y;
    if(a==1){
        temp= 0x80 + b - 1;
        z= temp>>4;
        y= temp & 0x0F;
        LCD_Cmd (z); 
         LCD_Cmd (y);
    }
    else if(a==2){
        temp= 0xC0 + b - 1;
        z= temp>>4;
        y= temp & 0x0F;
        LCD_Cmd (z); 
         LCD_Cmd (y);
    }
    
}
void LCD_Print_Char(char data){
    char Lower_Nibble,Upper_Nibble;
    Lower_Nibble = data & 0x0F;
    Upper_Nibble = data & 0xF0;
    RS=1;
    LCD_SetBit(Upper_Nibble>>4);
    EN=1;
    for(int i=2130483; i<=0; i--)NOP();
    EN=0;
    LCD_SetBit(Lower_Nibble);
    EN=1;
    for(int i=2130483; i<=0; i--)NOP();
    EN=0;
    
}
   
void LCD_Print_String(char *a){
    int i;
    for(i=0; a[i]!='\0'; i++)
        LCD_Print_Char(a[i]);
}

void beep(int c){
    for(int b=0; b<c; b++){
        buzzer=1;
         __delay_ms(50);
         buzzer=0;
          __delay_ms(50);
    }
}

void main(){
    TRISD=0x00;
    TRISC=0x00;
    TRISB=1;
    //PORTB=1;
   unsigned int a;
   
   StartButton=1;
   ModeButton=1;
    power=1;
    ChangeMode=1;
    pump=0;
    pumping=0;
    filled=0;
     LCD_Start();
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Print_String("Welcome to NCAT");
    LCD_Set_Cursor(2,1);
    LCD_Print_String("  MTCE Workshop");
    __delay_ms(200);
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Print_String("     3 MODE     ");
    LCD_Set_Cursor(2,1);
    LCD_Print_String(" Bottle Filling ");
    __delay_ms(200);
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Print_String("     Machine     ");
    LCD_Set_Cursor(2,1);
    LCD_Print_String("  Starting");
    __delay_ms(250);
    LCD_Set_Cursor(2,11);
    LCD_Print_String(".");
     __delay_ms(250);
    LCD_Set_Cursor(2,12);
    LCD_Print_String(".");
     __delay_ms(250);
    LCD_Set_Cursor(2,13);
    LCD_Print_String(".");
     __delay_ms(250);
    LCD_Set_Cursor(2,14);
    LCD_Print_String(".");
    beep(1);
    LCD_Clear();
    
    while(1){
        while(StartButton==1){
           LCD_Set_Cursor(2,1);
    LCD_Print_String("  Place Bottle  ");
    break;
    
        }
        filled=0;
        if(StartButton==0 && Bcheck==0){
            LCD_Set_Cursor(2,1);
            LCD_Print_String("Error! No Bottle");
            beep(3);
            __delay_ms(200);
        }
          while(Bcheck==1){
           LCD_Set_Cursor(2,1);
    LCD_Print_String("   Push Start   "); 
    if(StartButton==0 && ChangeMode==1 && Bcheck==1){
        __delay_ms(200);
        beep(2);
        LCD_Set_Cursor(2,1);
    LCD_Print_String("   Filling...   "); 
             pump=1;
             pumping=1;
             __delay_ms(3000);
             pump=0;
             pumping=0;
             __delay_ms(200);
             filled=1;
             
            
             LCD_Set_Cursor(2,1);
    LCD_Print_String("   Completed    ");
             while(Bcheck=1){
                 if(StartButton==0){
                     LCD_Set_Cursor(2,1);
    LCD_Print_String("Place New Bottle");
    beep(3);
    __delay_ms(200);
    LCD_Set_Cursor(2,1);
    LCD_Print_String("   Completed    "); 
    StartButton=1;
                 }
             }
        
    }
      if(StartButton==0 && ChangeMode==2 && Bcheck==1){
        __delay_ms(200);
        beep(2);
        LCD_Set_Cursor(2,1);
    LCD_Print_String("   Filling...   "); 
             pump=1;
             pumping=1;
             __delay_ms(2000);
             pump=0;
             pumping=0;
             __delay_ms(1000);
             filled=1;
             LCD_Set_Cursor(2,1);
    LCD_Print_String("   Completed    ");
             while(Bcheck=1){
                 if(StartButton==0){
                     LCD_Set_Cursor(2,1);
    LCD_Print_String("Place New Bottle");
    __delay_ms(200);
    beep(3);
    LCD_Set_Cursor(2,1);
    LCD_Print_String("   Completed    "); 
    StartButton=1;
                 }
             }
        
    }
      if(StartButton==0 && ChangeMode==3 && Bcheck==1){
        __delay_ms(200);
        beep(2);
        LCD_Set_Cursor(2,1);
    LCD_Print_String("   Filling...   "); 
             pump=1;
             pumping=1;
             __delay_ms(3000);
             pump=0;
             pumping=0;
             __delay_ms(1000);
             filled=1;
             LCD_Set_Cursor(2,1);
    LCD_Print_String("   Completed    ");
             while(Bcheck=1){
                 if(StartButton==0){
                     LCD_Set_Cursor(2,1);
    LCD_Print_String("Place New Bottle");
    __delay_ms(200);
    beep(3);
    LCD_Set_Cursor(2,1);
    LCD_Print_String("   Completed    "); 
    StartButton=1;
                 }
             }
        
    }
    
        }
        if(ModeButton==0){
            
            ChangeMode=ChangeMode+1;
            beep(1);
             
            
        }
        
        if(ChangeMode==1){
            Mode1=1;
            Mode2=0;
            Mode3=0;
            LCD_Set_Cursor(1,1);
            LCD_Print_String(" MODE 1 = 250ml ");
        }
         if(ChangeMode==2){
            Mode1=0;
            Mode2=1;
            Mode3=0;
            
            LCD_Set_Cursor(1,1);
            LCD_Print_String(" MODE 2 = 350ml ");
        }
        if(ChangeMode==3){
            Mode1=0;
            Mode2=0;
            Mode3=1;
            
            LCD_Set_Cursor(1,1);
            LCD_Print_String(" MODE 3 = 500ml ");
        }
        if(ChangeMode>=4){
            ChangeMode=1;
        }

    }
}
