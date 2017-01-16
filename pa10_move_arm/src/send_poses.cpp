#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Pose.h>


int main(int argc, char* argv[]) {
	ros::init(argc, argv, "send_poses");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<geometry_msgs::PoseArray>("/Coord", 1);
	int cpt = 0;
	ros::Rate loop(1000);
	while (ros::ok() && cpt < 2) {
		geometry_msgs::PoseArray poseArr;
		geometry_msgs::Pose p;
		p.orientation.w = 1.0;
		p.orientation.x = 0.0;
		p.orientation.y = 0.0;
		p.orientation.z = 0.0;
		p.position.x = -0.5;
		p.position.y = -0.5;
		p.position.z = 0.5;
		poseArr.poses.push_back(p);
		p.position.x = -0.2;
		poseArr.poses.push_back(p);
		p.position.x = 0.0;
		poseArr.poses.push_back(p);/*
		p.position.x = 0.1;
		poseArr.poses.push_back(p);*/
		poseArr.header.frame_id = "world";
		poseArr.header.stamp = ros::Time::now();
		pub.publish(poseArr);
		ROS_INFO("msg sent");
		cpt++;
		
		ros::spinOnce();
		loop.sleep();
	}
	
	return 0;
}