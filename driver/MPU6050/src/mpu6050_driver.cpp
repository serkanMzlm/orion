extern "C" {
    #include <errno.h>
    #include <fcntl.h>
    #include <i2c/smbus.h>
    #include <linux/i2c-dev.h>
    #include <string.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
}

#include <iomanip>

#include "mpu6050_driver.hpp"

MPU6050::MPU6050(int device_number){
    snprintf(file_name, 11, "/dev/i2c-%d", device_number);
    if(begin() < 0){
        exit(1);
    }
    readRangeConfig();
}

MPU6050::~MPU6050(){
    close(fd);
}

void MPU6050::setGyroRange(GyroRange_e range){
    int result = i2c_smbus_write_byte_data(fd, GYRO_CONFIG, range << 3);
    if (result < 0) reportError();
    ranges[GYR_R] = gyro_ranges[static_cast<size_t>(range)];
}

void MPU6050::setAccelRange(AccelRange_e range){
    int result = i2c_smbus_write_byte_data(fd, ACC_CONFIG, range << 3);
    if (result < 0) reportError();
    ranges[ACC_R] = accel_ranges[static_cast<size_t>(range)];
}

void MPU6050::setDlpfBandwidth(DlpfBandwidth_e bandwidth){
    int result = i2c_smbus_write_byte_data(fd, DLPF_CONFIG, bandwidth);
    if (result < 0) reportError();
    ranges[DLPF_R] = dlpf_ranges[static_cast<size_t>(bandwidth)];
}

void MPU6050::setGyroOffset(double offset_x, double offset_y, double offset_z){
	gyro_offset[X] = offset_x;
	gyro_offset[Y] = offset_y;
	gyro_offset[Z] = offset_z;
}

void MPU6050::setAccOffset(double offset_x, double offset_y, double offset_z){
	acc_offset[X] = offset_x;
	acc_offset[Y] = offset_y;
	acc_offset[Z] = offset_z;
}

double MPU6050::getAccelerationX() const{
    int16_t acc_x = read16(ACC_X_H);
	double accel_x_converted = convertRawAccData(acc_x);
	if (calibrated) {
		return accel_x_converted - acc_offset[X];
	}
	return accel_x_converted;
}

double MPU6050::getAccelerationY() const{
    int16_t acc_y = read16(ACC_Y_H);
    double accel_y_converted = convertRawAccData(acc_y);
	if (calibrated) {
		return accel_y_converted - acc_offset[Y];
	}
	return accel_y_converted;
}

double MPU6050::getAccelerationZ() const{
    int16_t acc_z = read16(ACC_Z_H);
    double accel_z_converted = convertRawAccData(acc_z);
	if (calibrated) {
		return accel_z_converted - acc_offset[Z];
	}
	return accel_z_converted;
}

double MPU6050::getAngularVelocityX() const{
	int16_t gyro_x = read16(GYRO_X_H);
	double gyro_x_converted = convertRawGyroData(gyro_x);
	if (calibrated) {
		return gyro_x_converted - gyro_offset[X];
	}
	return gyro_x_converted;
}

double MPU6050::getAngularVelocityY() const{
	int16_t gyro_y = read16(GYRO_Y_H);
	double gyro_y_converted = convertRawGyroData(gyro_y);
	if (calibrated) {
		return gyro_y_converted - gyro_offset[Y];
	}
	return gyro_y_converted;
}

double MPU6050::getAngularVelocityZ() const{
	int16_t gyro_z = read16(GYRO_Z_H);
	double gyro_z_converted = convertRawGyroData(gyro_z);
	if (calibrated) {
		return gyro_z_converted - gyro_offset[Z];
	}
	return gyro_z_converted;
}

int MPU6050::begin(){
    fd = open(file_name, O_RDWR);
    if(fd < 0){
        reportError("Could not open the I2C device");
		return -1;
    }
    if(ioctl(fd, I2C_SLAVE, MPU6050_ADDR) < 0){
        close(fd);
        reportError("Could not set I2C device address");
		return -1;
	}
    if(i2c_smbus_write_byte_data(fd, PWR_MGMT_1, 0) < 0){
        reportError("PWR_MGMT_1 could not be written");
        return -1;
    }
    return 1;
}

void MPU6050::readRangeConfig(){
    for(int i = 0; i < RANGE_ALL; i++){
        int result = i2c_smbus_read_byte_data(fd, DLPF_CONFIG + i);
        if(result < 0){
            reportError("Configuration reading process failed (" + std::to_string(i) + ")" );
        }
        result = i == 0 ? result & 7 : result >> 3;
        switch (i){
            case 0:
                ranges[DLPF_R] = dlpf_ranges[result];
                break;
            case 1:
                ranges[GYR_R] = gyro_ranges[result];
                break;
            case 2:
                ranges[ACC_R] = accel_ranges[result];
                break;
        }
    }
}

double MPU6050::convertRawAccData(int16_t accel_raw) const{
    const double acc = static_cast<double>(accel_raw) / accel_map.at(ranges[ACC_R])/* * GRAVITY */;
    return acc; // (m/s^2)
}

double MPU6050::convertRawGyroData(int16_t gyro_raw) const{
    const double gyro = static_cast<double>(gyro_raw) / gyro_map.at(ranges[GYR_R]) ;
    return gyro; //angular velocity (deg/s)
}

void MPU6050::calibrate(){
    for(int i = 0; i <= COUNT; ++i){
		gyro_offset[X] += getAngularVelocityX();
		gyro_offset[Y] += getAngularVelocityY();
		gyro_offset[Z] += getAngularVelocityZ();
		acc_offset[X] += getAccelerationX();
		acc_offset[Y] += getAccelerationY();
		acc_offset[Z] += getAccelerationZ();
		usleep(200);

		float percentage = static_cast<float>(i) / COUNT * 100.0; 
		std::cout << "\r" << "|";
		int pos = barWidth * percentage / 100.0;
		for (int j = 0; j < barWidth; ++j) {
			if (j < pos) {
				std::cout << "#";
			} else {
				std::cout << ".";
			}
		}
		std::cout << "| " << i << "/" << COUNT << " [" << std::fixed << 
											std::setprecision(2) << percentage << "%]";
		std::flush(std::cout);
	}
	std::cout << std::endl;

	gyro_offset[X] /= COUNT;
	gyro_offset[Y] /= COUNT;
	gyro_offset[Z] /= COUNT;
	acc_offset[X] /= COUNT;
	acc_offset[Y] /= COUNT;
	acc_offset[Z] /= COUNT;
	acc_offset[Z] -= 1;
	// acc_offset[Z] -= GRAVITY;
	calibrated = true;
}

void MPU6050::printConfig() const{
    std::cout << "Accelerometer Range: +-" << ranges[ACC_R] << "g\n";
    std::cout << "Gyroscope Range: +-" << ranges[GYR_R] << " degree per sec\n";
    std::cout << "DLPF Range: " << ranges[DLPF_R] << " Hz\n";
}

void MPU6050::printOffset() const{
    std::cout << "Accelerometer Offsets: x: " << acc_offset[X] << ", y: " << acc_offset[Y]
            << ", z: " << acc_offset[Z] << "\n";
    std::cout << "Gyroscope Offsets: x: " << gyro_offset[X] << ", y: " << gyro_offset[Y]
            << ", z: " << gyro_offset[Z] << "\n";
}

void MPU6050::printAcceleration() const{
	std::cout << "Acc\n";
	std::cout << "X: " << getAccelerationX() << "\n";
	std::cout << "Y: " << getAccelerationY() << "\n";
	std::cout << "Z: " << getAccelerationZ() << "\n";
}

void MPU6050::printAngularVelocity() const{
	std::cout << "Gyro\n";
	std::cout << "X: " << getAngularVelocityX() << "\n";
	std::cout << "Y: " << getAngularVelocityY() << "\n";
	std::cout << "Z: " << getAngularVelocityZ() << "\n";
}

void MPU6050::cleanTerminal() const{
    std::cout << "\033[8A";
}

void MPU6050::reportError(std::string error_info){
    std::cerr << "Error! " << error_info << ": " << strerror(errno);
}

int16_t MPU6050::read16(uint8_t reg) const{
    int16_t data_h = i2c_smbus_read_byte_data(fd, reg);
	int16_t data_l = i2c_smbus_read_byte_data(fd, reg + 1);
	return (data_l | data_h << 8);
}
