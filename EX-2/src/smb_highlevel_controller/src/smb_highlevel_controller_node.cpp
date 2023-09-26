#include <ros/ros.h>
#include "smb_highlevel_controller/SmbHighlevelController.hpp"
#include <sensor_msgs/LaserScan.h>
#include <sstream>
void scanCallback(const sensor_msgs::LaserScan& msg)
{
  int size = msg.ranges.size();
  float min = msg.range_max;

  for (int i = 0; i < size; i++) {
    if (msg.ranges.at(i) < min && msg.ranges.at(i) > msg.range_min) {
      min = msg.ranges.at(i);
    }
  }

  ROS_INFO_STREAM("Minimum distance: " << min);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "smb_highlevel_controller");
  ros::NodeHandle nodeHandle("~");
  smb_highlevel_controller::SmbHighlevelController smbHighlevelController(nodeHandle);
  ros::Subscriber subscriber = nodeHandle.subscribe("/scan", 10, scanCallback);
  ros::spin();
  return 0;
}
