cmake_minimum_required(VERSION 3.0)
project(MPU6050)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(mpu6050_lib ${CMAKE_CURRENT_SOURCE_DIR}/src/mpu6050_driver.cpp)
target_include_directories(mpu6050_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(mpu6050_lib PUBLIC i2c)

add_executable(mpu6050 ${CMAKE_SOURCE_DIR}/src/mpu6050.cpp)
target_link_libraries(mpu6050 mpu6050_lib)