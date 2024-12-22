cmake_minimum_required(VERSION 3.8)
project(serial_interface)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_library(wiringPi_LIB wiringPi)

# Generate ROS interfaces
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/MotorControl.srv"
)

# Find serial_interface after generating it
find_package(serial_interface REQUIRED)

# Add include directories
INCLUDE_DIRECTORIES(include)

# Add the executable
add_executable(motor_controller src/motor_control_server.cc src/serial_interface.cc)

# Ensure all dependencies are correctly linked
if(DEFINED ENV{RASP5})
  target_link_libraries(motor_controller ${wiringPi_LIB})
endif()
ament_target_dependencies(motor_controller rclcpp serial_interface)

# Install targets
install(TARGETS
  motor_controller
  DESTINATION lib/${PROJECT_NAME})

ament_package()

