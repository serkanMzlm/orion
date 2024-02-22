#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

extern "C"{
#include <errno.h>
#include <i2c/smbus.h>
}

#include "lcd_driver.hpp"

///////////////////////////////////
//         definition            //
///////////////////////////////////
LCD::LCD(void){
    std::cout << "The file has been launched." << std::endl;
    snprintf(filename, 11, "/dev/i2c-%d", i2c_bus);
    if(init(filename, LCD_SLAVE_ADDR)){
        exit(-1);
    }
    begin();
}

LCD::~LCD(){
    close(fd);
}

int LCD::init(const char* filename, int slave_addr){
    fd = open(filename, O_RDWR);
    if(fd < 0){
        reportError(errno, "Failed  to open I2C device.");
        return FAILED;
    }
    if(ioctl(fd, I2C_SLAVE, slave_addr)){
        reportError(errno, "Failed to connect to the I2C device");
        close(fd);
        return FAILED;
    }
    return OK;
}

void LCD::begin(){
    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    sleep(50);
    expanderWrite(_backlightval);
    sleep(1000);

    write4bits(0x03 << 4);
    sleep(5);
    write4bits(0x03 << 4);
    sleep(5);
    write4bits(0x03 << 4);
    sleep(5);
    write4bits(0x02 << 4);
    command(LCD_FUNCTIONSET | _displayfunction);
    _displaycontrol = LCD_DISPLAYON;
    display(ON);
    clear();
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
    home();
}

void LCD::clear(){
    command(LCD_CLEARDISPLAY);
	sleep(2);
}

void LCD::home(){
    command(LCD_RETURNHOME);
	sleep(2);
}

void LCD::setCursor(uint8_t col, uint8_t row){
    int row_offsets[] = { 0x00, 0x40 };
    if(row > _rows && row < 0){
        row = _rows-1;
    }
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD::display(State_e state){
    if(state == ON) _displaycontrol |= LCD_DISPLAYON;
    else _displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD::cursor(State_e state){
    if(state == ON) _displaycontrol |= LCD_CURSORON;
    else _displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD::blink(State_e state){
    if(state == ON) _displaycontrol |= LCD_BLINKON;
    else _displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD::autoscroll(State_e state){
    if(state == ON) _displaymode |= LCD_ENTRYSHIFTINCREMENT;
	else _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LCD::backlight(State_e state) {
    if(state == ON) _backlightval = LCD_BACKLIGHT;
	else _backlightval = LCD_NOBACKLIGHT;
	expanderWrite(_backlightval);
}

void LCD::scrollDisplayLeft(){
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE);
}

void LCD::scrollDisplayRight(){
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD::leftToRight(){
    _displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LCD::rightToLeft(){
    _displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LCD::createChar(uint8_t location, uint8_t charmap[]){
    location &= 0x7; 
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		command(charmap[i], Rs);
	}
}

int LCD::writeString(char* text){
    unsigned char ucTemp[2];
    int i = 0;
    if(fd < 0 || text == NULL){
        return FAILED;
    }

    while (i < 16 && *text) {
        ucTemp[0] = _backlightval | 1 | (*text & 0xf0);
		write(fd, ucTemp, 1);
        sleep(5);
        ucTemp[0] |= 4;
        write(fd, ucTemp, 1);
		sleep(5);
		ucTemp[0] &= ~4;
		write(fd, ucTemp, 1);
		sleep(5);
        ucTemp[0] = _backlightval | 1 | (*text << 4);
		write(fd, ucTemp, 1);
		ucTemp[0] |= 4; // pulse E
        write(fd, ucTemp, 1);
        sleep(5);
        ucTemp[0] &= ~4;
        write(fd, ucTemp, 1);
        usleep(5);
		text++;
		i++;
    }
}

//////////////////////////////////////
void LCD::write4bits(uint8_t value){
    expanderWrite(value);
	pulseEnable(value);
}

int LCD::expanderWrite(uint8_t data){
    if(write(fd, &data, 1) != 1){
        reportError(errno, "Error writing data over I2C");
        return FAILED;
    }
    return OK;
}

void LCD::pulseEnable(uint8_t data){
    expanderWrite(data | En);
    usleep(1);
    expanderWrite(data & ~En);
    usleep(50);
}

void LCD::command(uint8_t value, uint8_t mode){
	write4bits((value & 0xf0) | mode);
	write4bits(((value<<4) & 0xf0) | mode);
}

/////////////////////////////////////////
void LCD::sleep(uint16_t ms_time){
    usleep(ms_time * 1000);
}

void LCD::reportError(int error, std::string info){
    std::cerr << "Error! " << info << " : " << strerror(error);
}