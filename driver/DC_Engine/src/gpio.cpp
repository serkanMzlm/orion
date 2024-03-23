#include <unistd.h> 
#include <stdio.h>
#include "gpio_driver.hpp"

#define DELAY 1000
int pin = 17;
int main(){
    unexportPin(pin);
    usleep(DELAY);
    exportPin(pin);
    usleep(DELAY);
    directionPin(pin, OUTPUT);
    usleep(DELAY);
    writePin(pin, 1);
    getchar();
    writePin(pin, 0);
    usleep(DELAY);
    unexportPin(pin);
    return 0;
}