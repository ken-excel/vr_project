#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <fstream>

using namespace std;

ofstream outputFile;
nav_msgs::Odometry odom_msg;

double getYaw(geometry_msgs::Pose &msg)	
{
	tf::Quaternion q( 
    	msg.orientation.x, 
    	msg.orientation.y,
    	msg.orientation.z,
    	msg.orientation.w);
	tf::Matrix3x3 m(q); 
	double roll, pitch, yaw;
	m.getRPY(roll, pitch, yaw); 
	return yaw;
}

void readOdom(const nav_msgs::Odometry::ConstPtr& msg){
	odom_msg = *msg;
}

int main(int argc, char** argv){
	ros::init(argc, argv, "path_tracker");
	ros::NodeHandle nh_;
	ros::Subscriber odom_sub_ = nh_.subscribe("/odom", 1, &readOdom);

	ros::Time begin_time = ros::Time::now();
	outputFile.open("log.txt");

	ros::Rate rate(5);
	while(ros::ok())
	{
		ros::spinOnce();
		double elapsed = (ros::Time::now() - begin_time).toSec();
		outputFile<<elapsed<<","<<odom_msg.pose.pose.position.x<<","<<odom_msg.pose.pose.position.y<<","<<getYaw(odom_msg.pose.pose)<<endl;
	}
	outputFile.close();
	return 0;
}
