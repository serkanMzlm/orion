cmake_minimum_required(VERSION 3.0)
project(servo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILE_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(servo_lib ${CMAKE_CURRENT_SOURCE_DIR}/src/servo_driver.cpp)
target_include_directories(servo_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(servo ${CMAKE_SOURCE_DIR}/src/servo.cpp)
target_link_libraries(servo servo_lib)