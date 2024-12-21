#include "rclcpp/rclcpp.hpp"
#include "serial_interface/srv/motor_control.hpp"

#include "serial_interface/serial_interface.hh"

#include <memory>

SerialInterface* si;

struct motor_velocity {
  float mv1;
  float mv2;
  float mv3;
};

void change_motor_velocity(
  const std::shared_ptr<serial_interface::srv::MotorControl::Request> request,
  std::shared_ptr<serial_interface::srv::MotorControl::Response> response
) {
  motor_velocity m;
  m.mv1 = request->mv1;
  m.mv2 = request->mv2;
  m.mv3 = request->mv3;

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
              "Recieved Motor Control speeds: mv1: %.6f - mv2: %.6f - mv3: %.6f\n",
              m.mv1, m.mv2, m.mv3);

  char* msg = reinterpret_cast<char*>(&m);
  si->send(msg);

  response->ack = true;
}

int main(int argc, char**argv) {
  rclcpp::init(argc, argv);

  si = new SerialInterface("/dev/ttyACM0");
  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("motor_control_server");

  rclcpp::Service<serial_interface::srv::MotorControl>::SharedPtr service =
    node->create_service<serial_interface::srv::MotorControl>("serial_interface/change_motor_velocity", change_motor_velocity);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to recieve motor instructions");

  rclcpp::spin(node);
  rclcpp::shutdown();
}
