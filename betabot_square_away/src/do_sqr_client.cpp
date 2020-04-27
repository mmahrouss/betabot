#include "ros/ros.h"                          // ROS Default Header File
#include "betabot_square_away/BetabotCustomServiceMessage.h"

int main(int argc, char **argv)                // Node Main Function
{
    ros::init(argc, argv, "do_sqr_client");   // Initializes Node Name

    ros::NodeHandle nh;                          // Node handle declaration
    ros::ServiceClient get_closest = nh.serviceClient<betabot_square_away::BetabotCustomServiceMessage>("move_betabot_in_square_custom");

    if(!get_closest.waitForExistence(ros::Duration(5))){
        ROS_ERROR("do_sqr server did not come up in 5 seconds!");
        return 1; // to exit if no server is running
    };

    betabot_square_away::BetabotCustomServiceMessageRequest clo_req;
    betabot_square_away::BetabotCustomServiceMessageResponse clo_res;
    for (int i = 1; i < argc; i += 2){
        clo_req.side = atof(argv[i]);
        clo_req.repetitions = atoi(argv[i+1]);

        get_closest.call(clo_req,clo_res);
    }

    return 0;
}