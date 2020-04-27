#define _USE_MATH_DEFINES
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include <random>


class RandomWalk{
  ros::Publisher pub;
  std::mt19937 gen;
  std::bernoulli_distribution dis;
  std::uniform_real_distribution<> t_dis;
  public:
  RandomWalk(ros::Publisher p)
  {
    pub = p;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::bernoulli_distribution dis(0.5); //Bernolly uniform distribution
    std::uniform_real_distribution<> t_dis(0.3, 1); //uniform distribution
  }
  void randomWalk(const sensor_msgs::LaserScan &msg)
  {
    // ROS_INFO("YEP");
    geometry_msgs::Twist twst;
    // initialize the twist with zeros
    twst.linear.x =  twst.linear.y = twst.linear.z = 
    twst.angular.x = twst.angular.y = twst.angular.z = 0;
    // Calculate the LaserScan reading forward w. r. t. the robot.
    // Calculate the angle at which the closest object is at
    auto mindst_angle = std::min_element( msg.ranges.begin(), msg.ranges.end());
    // Get the min value from the iterator
    float min_value = *mindst_angle;
    if (min_value < 0.3)
    {
      //Pointer Difference to get the index of that angle
      auto index = mindst_angle - msg.ranges.begin();
      // angle of the closest laser reading in radians
      float a_closst_d = index * msg.angle_increment;
      // Move back opposite to the object
      twst.linear.x = -.4 * std::cos(a_closst_d);
      twst.linear.y = -.4 * std::sin(a_closst_d);
      // publish the velocity
      pub.publish(twst);
      // keep going back for a random time.
      ros::Duration(t_dis(gen)).sleep();
      return;
    }
    // Move forward
    twst.linear.x = 0.5;
    // Random cw or ccw 
    twst.angular.z = (dis(gen))? 1.5 : -1.5;
    // publish the velocity
    pub.publish(twst);
    // keep going for a random time.
    ros::Duration(t_dis(gen)).sleep();
    return;
  }
};

int main(int argc, char **argv)
{    
  ros::init(argc, argv, "random_walk");   // Initializes Node Name
  ros::NodeHandle nh; // Define node handle 
  ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1); //publish velocity twist
  RandomWalk rw(vel_pub); //init the RandomWalk
  ros::Subscriber sub = nh.subscribe("/scan", 1, &RandomWalk::randomWalk, &rw); //Subscribe to the laser scan
  ros::spin(); //Keep the node alive
  return 0;
}