#include <unistd.h> 
#include <stdio.h>
#include "gpio_driver.hpp"

#define DELAY 1000
int pin = 17;
int main(){
    exportPin(pin);
    usleep(DELAY);
    directionPin(pin, OUTPUT);
    usleep(DELAY);
    writePin(pin, 1);
    getchar();
    writePin(pin, 0);
    return 0;
}