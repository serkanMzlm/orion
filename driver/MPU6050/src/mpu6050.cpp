#include "mpu6050_driver.hpp"
#include <signal.h>

void handleCtrlC(int signum) {
    std::cout << "Ctrl+C program terminated" << std::endl;
    exit(signum);
}

int main(){
    MPU6050 mpu6050;
    for(int i = 0; i < 10; i++) {
        std::cout << "\n";
    }

    signal(SIGINT, handleCtrlC);

    while (true) {
        if(mpu6050.getCalibration()){
            mpu6050.setCalibration(false);
            mpu6050.printConfig();
            mpu6050.printOffset();
        }
        mpu6050.cleanTerminal();
        mpu6050.printAcceleration();
        mpu6050.printAngularVelocity();

        usleep(10000);
    }

    return 0;
}