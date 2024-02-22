cmake_minimum_required(VERSION 3.0)
project(gpio)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(gpio_lib ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_driver.cpp)
target_include_directories(gpio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(gpio ${CMAKE_SOURCE_DIR}/src/gpio.cpp)
target_link_libraries(gpio gpio_lib)