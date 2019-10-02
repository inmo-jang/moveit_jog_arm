#include "geometry_msgs/TwistStamped.h"
#include "control_msgs/JointJog.h"
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "rain_unity/rain_system.h"
#include "rain_unity/Human_orion.h"
#include "rain_unity/Hand_orion.h"

namespace to_twist
{
class leapToTwist
{
public:
  leapToTwist() : spinner_(1)
  {
    leap_sub_ = n_.subscribe("/rain/leap_motion", 1, &leapToTwist::leapCallback, this);
    twist_pub_ = n_.advertise<geometry_msgs::TwistStamped>("jog_server/delta_jog_cmds", 1);

    spinner_.start();
    ros::waitForShutdown(); 
  };

private:
  ros::NodeHandle n_;
  ros::Subscriber leap_sub_;
  ros::Publisher twist_pub_;
  ros::AsyncSpinner spinner_;

  // Convert incoming joy commands to TwistStamped commands for jogging
//   void leapCallback(const sensor_msgs::Joy::ConstPtr& msg)
  void leapCallback(const rain_unity::Human_orion::ConstPtr& msg)
  {
    rain_unity::Human_orion LeapMsg_from_unity = *msg;
    ////  TODO: Left Hand

    // Cartesian jogging
    geometry_msgs::TwistStamped twist;
    twist.header.stamp = ros::Time::now();

    // Right Hand
    if (LeapMsg_from_unity.right_hand.is_present){
        // Coordination transformation from Unity to ROS
        // e.g. ROS (x,y,z) = Unity (z, -x, y)
        
        twist.twist.linear.x = LeapMsg_from_unity.right_hand.palm_velocity[2];
        twist.twist.linear.y = -LeapMsg_from_unity.right_hand.palm_velocity[0];
        twist.twist.linear.z = LeapMsg_from_unity.right_hand.palm_velocity[1];



        // twist.twist.angular.x = -msg->axes[3];
        // twist.twist.angular.y = msg->axes[4];
        // twist.twist.angular.z = -msg->buttons[0] + msg->buttons[1]; // yaw


    }
    




    twist_pub_.publish(twist);

  }
};
}  // end to_twist namespace

int main(int argc, char** argv)
{
  ros::init(argc, argv, "leapmotion_to_twist");

  to_twist::leapToTwist to_twist;

  return 0;
}
