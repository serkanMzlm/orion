#ifndef __MPU6050_TYPE_HPP__
#define __MPU6050_TYPE_HPP__

#define DLPF_CONFIG (0x1A)
#define GYRO_CONFIG (0x1B)
#define ACC_CONFIG (0x1C)

#define ACC_X_H (0x3B) 
#define ACC_Y_H (0x3D)
#define ACC_Z_H (0x3F)

#define TEMP_H (0x42)

#define GYRO_X_H (0x43)
#define GYRO_Y_H (0x45)
#define GYRO_Z_H (0x47)
 
#define PWR_MGMT_1 (0x6B)
#define MPU6050_ADDR (0x68)

#define GRAVITY (9.81f)
#define COUNT (1000)

#ifndef M_PI 
    #define M_PI (3.14159265359) 
#endif

typedef enum { 
    ACC_2_G, 
    ACC_4_G, 
    ACC_8_G, 
    ACC_16_G 
}AccelRange_e;

typedef enum { 
    GYR_250_DPS, 
    GYR_500_DPS, 
    GYR_1000_DPS, 
    GYR_2000_DPS 
}GyroRange_e;

typedef enum {
    DLPF_260_HZ, 
    DLPF_184_HZ,
    DLPF_94_HZ, 
    DLPF_44_HZ,
    DLPF_21_HZ, 
    DLPF_10_HZ,
    DLPF_5_HZ
}DlpfBandwidth_e;

typedef enum { DLPF_R, GYR_R, ACC_R, RANGE_ALL }Range_e;
typedef enum { X, Y, Z, CARTESIAN_ALL }Cartesian_e;

#endif
