#include "rclcpp/rclcpp.hpp"
#include "serial_interface/srv/motor_control.hpp"

#include "serial_interface/serial_interface.hh"

#include <memory>

SerialInterface* si;

struct motor_velocity {
  uint32_t id;
  double velocity;
};

void change_motor_velocity(
  const std::shared_ptr<serial_interface::srv::MotorControl::Request> request,
  std::shared_ptr<serial_interface::srv::MotorControl::Response> response
) {
  motor_velocity m;
  m.id = request->id;
  m.velocity = request->velocity;

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
              "Recieved Motor ID: %i with Control speed: %.6f\n",
              m.id, m.velocity);

  // Make message
  int sz = sizeof(motor_velocity);
  char* msg = new char[sz];
  msg = reinterpret_cast<char*>(&m);

  #ifdef RASP5
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
              "Recieved %i bytes of data\n",
              serialDataAvail(si->fd));
  #endif

  si->send(msg, sz);

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
  delete si;

  return 0;
}
