#ifndef __LCD_DRIVER_HPP__
#define __LCD_DRIVER_HPP__

#include <unistd.h>
#include <cstring>
#include <iostream>

#include "lcd_type.hpp"
///////////////////////////////////
//         description           //
///////////////////////////////////
class LCD{
private:
    int fd;
    int i2c_bus = 1;
    char filename[11];
	uint8_t _cols = LCD_COLUMN;
	uint8_t _rows = LCD_ROW;
    uint8_t _addr = LCD_SLAVE_ADDR;
	uint8_t _backlightval = LCD_BACKLIGHT;
    uint8_t _displayfunction;
    uint8_t _displaymode;
    uint8_t _displaycontrol;
public:
    LCD(void);
    ~LCD();
    int init(const char* filename, int slave_addr = LCD_SLAVE_ADDR);
    void begin();
    void clear();
    void home();
    void setCursor(uint8_t col, uint8_t row);
    void display(State_e state);
    void cursor(State_e state);
    void blink(State_e state);
    void autoscroll(State_e state);
    void backlight(State_e state);
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void createChar(uint8_t location, uint8_t charmap[]);
    int writeString(char* text);
private:
    void write4bits(uint8_t value);
    int expanderWrite(uint8_t data);
    void pulseEnable(uint8_t data);
    void command(uint8_t value, uint8_t mode = 0);
private:
    void sleep(uint16_t ms_time);
    void reportError(int error, std::string info = "Errno");
};

#endif