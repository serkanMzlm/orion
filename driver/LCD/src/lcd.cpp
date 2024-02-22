#include "lcd_driver.hpp"

int main(){
    LCD lcd;
    std::cout << "Press Enter to exit." << std::endl;
    lcd.cursor(ON);
    lcd.blink(ON);
    lcd.backlight(ON);  
    lcd.writeString("Hello");
    lcd.setCursor(0, 1);
    lcd.writeString("World");
    getchar();
    lcd.clear();
    lcd.backlight(OFF);
    return 0;
}
