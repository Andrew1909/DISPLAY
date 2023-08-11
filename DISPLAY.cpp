#include <stdio.h>
//#include <conio.h>
//#include <dos.h>
#include <string.h>
#include <Arduino.h>

//#include "ex.h"
#include "DISPLAY.h"

int _displaycontrol;
int _displaymode;
int _rs_pin = 2;
int _rw_pin;
int _enable_pin = 3;
int _row_offsets[4];
int _data_pins[4];
int _displayfunction;
int _numlines = 4;

static void EXdisplay(int value, int mode);
int v;

uint8_t as(){
  return v;
}
DISPL_PARAM	DisplParam;

/***************************************************************************\
  <ConfigDisplay> DP-4x20 VF, нет курсора
\***************************************************************************/
void ConfigDisplay(void)
{

    _data_pins[0] = 4;
    _data_pins[1] = 5;
    _data_pins[2] = 6;
    _data_pins[3] = 7; 

  //set rows
    _row_offsets[0] = 0x00;
    _row_offsets[1] = 0x40;
    _row_offsets[2] = 0x00 + 20;
    _row_offsets[3] = 0x40 + 20;

    pinMode (_rs_pin, OUTPUT);
    pinMode (_enable_pin, OUTPUT);

    for (int i=0; i< 4; ++i)
    {
        pinMode (_data_pins[i], OUTPUT);
    } 
    delayMicroseconds(50000); 

    write4bits (0b00110000);//3
    delay(40);
    //EXdisplay (0b00000010,LOW); //function set//2
    delay(40);
    //EXdisplay (0b00000010, LOW); //function set//2
    delay(40);
  //set # lines, font size, etc.
    EXdisplay(LCD_FUNCTIONSET | LCD_4BITMODE| LCD_2LINE,LOW);  //function set 40
        delay(40);
    EXdisplay(LCD_FUNCTIONSET | LCD_4BITMODE| LCD_2LINE,LOW);  //function set 40
    delay(40);
    //EXdisplay (0b00000000,LOW);//display on 40
    EXdisplay(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF,LOW);//display on 40
    delay(40);
    ClsDisplay();//clear 12
    delay(40);
    //EXdisplay(0b00000101 ,LOW);//mode set 12
    EXdisplay(LCD_ENTRYMODESET |LCD_ENTRYLEFT ,LOW);//mode set 12
};

/***************************************************************************\
  <ClsDisplay>
\***************************************************************************/
void ClsDisplay(void)
{
    EXdisplay(0b00000001, LOW);
    delayMicroseconds(2000);  // this command takes a long time!
};

/***************************************************************************\
  <DisplayYX>
\***************************************************************************/
void DisplayYX(int y, int x, char *str)//x,y-координаты курсора 
{
    int i;
    const int max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
    if ( x >= max_lines ) {
        x = max_lines - 1;    // we count rows starting w/0
    }
    if ( x >= _numlines ) {
        x = _numlines - 1;    // we count rows starting w/0
    }
    EXdisplay(LCD_SETDDRAMADDR | (y + _row_offsets[x]), LOW);

    if (str == NULL) return 0;  //проверка на пустой указатель
    else{
        const uint8_t *buffer=(const uint8_t *)str;
        long size=strlen(str);
        while (size--) {
            if (write(*buffer++));
            else break;
        }
    }
}

long write(int value) {
    EXdisplay(value, HIGH);
    
    return value; // assume sucess
}

/***************************************************************************\
  <Display>
\***************************************************************************/
void Display(char *str)
{
    int i;
    for (i=0; i<strlen(str); i++)
       EXdisplay(*(str+i), 1);
}

/* *************************************************************
*	процедура вывода 7-ми битного кода в дисплей           *
** ************************************************************/

void a2(int value){  
  for (int i = 0; i < 4; i++) {
        digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }
    v=value;
    digitalWrite(_enable_pin, LOW);
    delayMicroseconds(70);    
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(70);    // enable pulse must be >450ns
}


static void EXdisplay(int value, int mode)
{  
   
		//!!!!! 29.06.07 ConfigPIO();
    digitalWrite(_rs_pin, mode);
    //write4bits(value>>4);//функция для инициализации. когда сумируют шеснадцатиричные чила, убрать лишнее, сдвинув на 4
    a2(value>>4);
    write4bits(value);
}



void write4bits(int value) {

    for (int i = 0; i < 4; i++) {
        digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }
    digitalWrite(_enable_pin, LOW);
    delayMicroseconds(40);    
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(40);    // enable pulse must be >450ns
}

void cmd_to_display(byte value){

  byte rs=(value>>5)&0x01;
  byte rw=(value>>4)&0x01;
  byte d7_pin=(value>>3)&0x01;
  byte d6_pin=(value>>2)&0x01;
  byte d5_pin=(value>>1)&0x01;
  byte d4_pin=(value>>0)&0x01;

  digitalWrite (_rs_pin, rs);
  digitalWrite (_rw_pin, rw);
  digitalWrite (_data_pins[0], d4_pin);
  digitalWrite (_data_pins[1], d5_pin);
  digitalWrite (_data_pins[2], d6_pin);
  digitalWrite (_data_pins[3], d7_pin);
}