#ifndef __MPU6050_DRIVER_HPP__
#define __MPU6050_DRIVER_HPP__

#include <string>
#include <iostream>
#include <unordered_map>

#include "mpu6050_type.hpp"

class MPU6050{
private:
    int fd;
    char file_name[11];
    bool calibrated = false;
    int barWidth = 50;

    int ranges[RANGE_ALL] = {260, 250, 2};
    // double gyro_offset[CARTESIAN_ALL] = {0.0, 0.0, 0.0};
    // double acc_offset[CARTESIAN_ALL] = {0.0, 0.0, 0.0};

    double gyro_offset[CARTESIAN_ALL] = {-10.54, -0.71, -0.64};
    double acc_offset[CARTESIAN_ALL] = {-0.07, 0.10, 0.05};

    const std::array<int, 4> accel_ranges{2, 4, 8, 16};
    const std::array<int, 4> gyro_ranges{250, 500, 1000, 2000};
    const std::array<int, 7> dlpf_ranges{260, 184, 94, 44, 21, 10, 5};

    const std::unordered_map<int, int> accel_map{{2, 16384}, {4, 8192}, {8, 4096}, {16, 2048}};
    const std::unordered_map<int, double> gyro_map{{250, 131}, {500, 65.5}, {1000, 32.8}, {2000, 16.4}};
public:
    MPU6050(int device_number = 1);
    ~MPU6050();

    void setGyroRange(GyroRange_e range);
    void setAccelRange(AccelRange_e range);
    void setDlpfBandwidth(DlpfBandwidth_e bandwidth);
    void setGyroOffset(double offset_x, double offset_y, double offset_z);
    void setAccOffset(double offset_x, double offset_y, double offset_z);

    double getAccelerationX() const;
    double getAccelerationY() const;
    double getAccelerationZ() const;
    double getAngularVelocityX() const;
    double getAngularVelocityY() const;
    double getAngularVelocityZ() const;

private:
    int begin();
    void readRangeConfig();
    double convertRawAccData(int16_t accel_raw) const;
    double convertRawGyroData(int16_t gyro_raw) const;
    void reportError(std::string error_info = "Errno");

    int16_t read16(uint8_t reg) const; 

public:
    void calibrate();
    void printConfig() const;
    void printOffset() const;
    void printAcceleration() const;
    void printAngularVelocity() const;
    void cleanTerminal() const;
};

#endif
