#define _USE_MATH_DEFINES
// #define XTENSOR_USE_XSIMD
#include <cmath>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "betabot_square_away/BetabotCustomServiceMessage.h"
#include "geometry_msgs/Quaternion.h"
// #include "xtensor/xarray.hpp"
// #include "xtensor-blas/xlinalg.hpp"
// #include <torch/script.h>
// #include <torch/extension.h>

class SquareDoer{
    public:
        SquareDoer(ros::Publisher vel_pub): vel_publisher(vel_pub), x_(0), y_(0) ,yaw(0)
        {}
        bool DoSquare(betabot_square_away::BetabotCustomServiceMessage::Request &request,
                      betabot_square_away::BetabotCustomServiceMessage::Response &response)
        {   
            double initial_x, initial_y, initial_yaw, d_angle;

            geometry_msgs::Twist twst;
            twst.linear.x =  twst.linear.y = twst.linear.z = 0;
            twst.angular.x = twst.angular.y = twst.angular.z = 0;
            double angle_rotated = 0;
            for (int rep =0; rep<request.repetitions && ros::ok(); rep++){
                for (int side = 0; side<4 && ros::ok(); side++){
                    initial_x = this->x_;
                    initial_y = this->y_;
                    initial_yaw = this->yaw;

                    twst.linear.x = 0.3;
                    vel_publisher.publish(twst);
                    while ((dist2d(initial_x, initial_y) <= request.side) && ros::ok()){
                        ros::spinOnce();
                    }
                    twst.linear.x = 0.0;
                    vel_publisher.publish(twst);
                    ros::Duration(0.001).sleep();
                    twst.angular.z = 1.2;
                    vel_publisher.publish(twst);
                    angle_rotated = 0;
                    do 
                    {   
                        ros::Duration(0.05).sleep();
                        ros::spinOnce();
                        // d_angle = angle_diff(this->yaw, initial_yaw);
                        angle_rotated += angle_diff(this->yaw, initial_yaw);
                        initial_yaw = this -> yaw;
                    // }while ((((d_angle >= -1e-1) && (d_angle < M_PI_2))
                    //      || ((d_angle < 0) && (d_angle < -3*M_PI_2))    ) && ros::ok());
                    }while( abs(angle_rotated + 1e-1) < M_PI_2 && ros::ok());
                    twst.angular.z = 0.0;
                    vel_publisher.publish(twst);
                    ros::Duration(0.001).sleep();
                }
            }
            twst.linear.x = 0.0;
            twst.angular.z = 0.0;
            vel_publisher.publish(twst);
            response.success = true;
            return true;
        }
        void OdomCallback(const nav_msgs::OdometryConstPtr &msg){
            x_ = msg->pose.pose.position.x;  // get the robot x pose from the odom topic
            y_ = msg->pose.pose.position.y;  // get the robot y pose from the odom topic
             //get the quaternion position. then use it to calculate yaw angle.
            yaw = _calc_yaw(msg->pose.pose.orientation);
            // if (yaw < 0) // convert from -pi< a < pi to 0< a < 2pi
            //     yaw +=  2*M_PI;
        }

    private:
        ros::Publisher vel_publisher;
        double x_;
        double y_;
        double yaw;
        // Ros already has a built in function for this
        double _calc_yaw(geometry_msgs::Quaternion q){
            double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
            double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
            return std::atan2(siny_cosp, cosy_cosp);
        }
        double dist2d(double x2, double y2){
            return std::sqrt(std::pow(x_- x2, 2) + std::pow(y_- y2, 2));
        }

        double angle_diff(double a1,double a2){
            double a =  a1 - a2;
            while (a > M_PI)
                a -= 2 * M_PI;
            while (a <  - M_PI)
                a += 2 * M_PI;
            return a;
        }
};

int main(int argc, char **argv)                // Node Main Function
{
    // at::Tensor a = at::ones({2, 2}, at::kInt);
    // at::Tensor b = at::randn({2, 2});
    // auto c = a + b.to(at::kInt);
    ros::init(argc, argv, "do_sqr_server");   // Initializes Node Name
    ros::NodeHandle nh;  
    ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    SquareDoer monitor(vel_pub);
    ros::ServiceServer do_sqr = nh.advertiseService("move_betabot_in_square_custom",&SquareDoer::DoSquare,&monitor);
    ros::Subscriber sub = nh.subscribe("/odom", 1, &SquareDoer::OdomCallback,&monitor);
    while(ros::ok())
        ros::spinOnce();                                 // Wait for ever for the service request
    return 0;
}