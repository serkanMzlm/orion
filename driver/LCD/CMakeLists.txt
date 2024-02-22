cmake_minimum_required(VERSION 3.0)
project(lcd)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILE_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(lcd_lib ${CMAKE_CURRENT_SOURCE_DIR}/src/lcd_driver.cpp)
target_include_directories(lcd_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(lcd ${CMAKE_SOURCE_DIR}/src/lcd.cpp)
target_link_libraries(lcd lcd_lib)