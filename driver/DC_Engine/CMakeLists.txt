cmake_minimum_required(VERSION 3.0)
project(DC)

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# add_library(gpio_lib ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_driver.cpp)
# target_include_directories(gpio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
find_library(GPIO_LIB gpio_lib HINTS /home/serkan/MyWorkspace/orion/driver/GPIO/build/libgpio_lib.so)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/gpio.cpp)
target_link_libraries(${PROJECT_NAME} ${gpio_lib})
target_include_directories(${PROJECT_NAME} PUBLIC /home/serkan/MyWorkspace/orion/driver/GPIO/inc)