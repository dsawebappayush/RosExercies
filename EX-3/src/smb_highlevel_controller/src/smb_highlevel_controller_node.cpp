#include <ros/ros.h>
#include "smb_highlevel_controller/SmbHighlevelController.hpp"
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>

// Define a global variable to store the pillar's position
geometry_msgs::PointStamped pillarPosition;
ros::Publisher pillarPositionPublisher;
 ros::Publisher cmdVelPublisher;
int min_distance_index=0;
float pillarAngle;
 ros::Subscriber subscriber;
float min;
void scanCallback(const sensor_msgs::LaserScan& msg)
{
  int size = msg.ranges.size();
  min = msg.range_max;

  for (int i = 0; i < size; i++) {
    if (msg.ranges.at(i) < min && msg.ranges.at(i) > msg.range_min) {
      min = msg.ranges.at(i);
      min_distance_index=i;
    }
  }

  ROS_INFO_STREAM("Minimum distance: " << min);

             double angle_increment = msg.angle_increment;

        // Calculate the angle to the pillar based on the index and angle increment
        double angle_to_pillar = msg.angle_min + min_distance_index * angle_increment;

        // Set the desired distance to the pillar (e.g., 1 meter)
        double desired_distance = 1.0;

        // Calculate the control signal (P controller)
        double error = desired_distance - min;

        // Control linear velocity in x direction (forward/backward motion)
        geometry_msgs::Twist cmd_vel;
       cmd_vel.linear.x = -.2* error;

        // Control angular velocity (rotation)
        cmd_vel.angular.z = .8 * angle_to_pillar;

        // Publish the control signal to move the robot
        cmdVelPublisher.publish(cmd_vel);

  // Publish the pillar position
  pillarPositionPublisher.publish(pillarPosition);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "smb_highlevel_controller");
  ros::NodeHandle nodeHandle("~");
  smb_highlevel_controller::SmbHighlevelController smbHighlevelController(nodeHandle);
  subscriber = nodeHandle.subscribe("/scan", 10, scanCallback);
  
  // Create a publisher to publish the pillar position
    cmdVelPublisher = nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  // Create a Twist message for controlling the robot's velocity
  
  pillarPositionPublisher = nodeHandle.advertise<geometry_msgs::PointStamped>("/pillar_position", 10);


 
  ros::spin();

  return 0;
}
