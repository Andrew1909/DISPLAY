#ifndef DISPLAY_h
#define DISPLAY_h

#define LCD_CLEARDISPLAY 0x01
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYLEFT 0x02

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00

// flags for function set
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08

typedef struct
{
    unsigned DisplRPMMin;
	unsigned DisplRPMMax;
	unsigned DisplRPMSegment;
} DISPL_PARAM;

extern DISPL_PARAM DisplParam;
uint8_t as();

  void ConfigDisplay(void); //настройка дисплея (init)
  void ClsDisplay(void);//очистка
  void DisplayYX(int y, int x, char *str);//вывод
  void Display(char *str);//4 bit print
  void write4bits(int value);
  long write(int value);
    void cmd_to_display(byte value);
#endif