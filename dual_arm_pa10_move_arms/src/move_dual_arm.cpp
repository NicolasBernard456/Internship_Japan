#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include "tf/transform_listener.h"
#include "tf/message_filter.h"
#include "message_filters/subscriber.h"
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <geometry_msgs/PoseArray.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>

// MoveIt!
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/GetStateValidity.h>


#include <std_srvs/SetBool.h>
#include <std_msgs/Float64.h>
#include <std_srvs/SetBoolRequest.h>
#include <std_srvs/SetBoolResponse.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>
#include <moveit/planning_interface/planning_interface.h>
#include <std_msgs/Bool.h>

#include "tf/message_filter.h"
#include <geometry_msgs/PointStamped.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <shape_msgs/SolidPrimitive.h>


ros::Publisher pub, pub2, pub3, planning_scene_diff_publisher;
ros::ServiceClient cl;

int main(int argc, char **argv){
	ros::init (argc, argv, "move_dual_arm");
	ros::AsyncSpinner spinner(1);
	spinner.start();
	ros::NodeHandle n;
	
	
//  	ros::Subscriber sub = n.subscribe("/Bool_move", 1, callback2);
	cl = n.serviceClient<std_srvs::SetBool>("Fin_mouvement");
	
	
	pub = n.advertise<geometry_msgs::PoseStamped>("/Pose_trajectory", 1);
	pub2 = n.advertise<geometry_msgs::PoseStamped>("/Pose_trajectory_origin", 1);
	ros::ServiceClient grip = n.serviceClient<std_srvs::SetBool>("Gripper_control");
	
	
	static moveit::planning_interface::MoveGroup group("left_arm");
// 	static moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
	static moveit::planning_interface::MoveGroup::Plan my_plan;
// 	std::cout << group.getCurrentPose().header.frame_id << " " << std::endl;
	while (ros::ok()) {
		bool success2 = false;
		try{
			std_srvs::SetBool b;
			b.request.data = false;
			grip.call(b);
			geometry_msgs::PoseStamped target_pose_link, target_pose_world;
		
	// 		planning_scene_interface.addCollisionObjects(attached_object);
			group.setPlannerId("RRTConnectkConfigDefault");
			group.setEndEffectorLink("gripper");
			group.setGoalOrientationTolerance(0.0001);
			group.setGoalPositionTolerance(0.0001);
			group.setGoalJointTolerance(0.0001);
			target_pose_link.pose.position.x = 0.2;
			target_pose_link.pose.position.y = 0;
			target_pose_link.pose.position.z = 0;
			tf::Quaternion q;
			
			q.setRPY(1.56, 3.14, 0);
			target_pose_link.pose.orientation.x = q.getX();
			target_pose_link.pose.orientation.y = q.getY();
			target_pose_link.pose.orientation.z = q.getZ();
			target_pose_link.pose.orientation.w = q.getW();
			target_pose_link.header.frame_id = "Approche";
	// 		ROS_INFO("OK");
		
			ros::Time t = ros::Time(0);
			tf::TransformListener tr;
			tf::StampedTransform transform;
			tr.setUsingDedicatedThread(true);
			
			
			int wait = 0;
// 			std::cout << std::endl;
			while (!tr.waitForTransform("/world", "/Approche", t,ros::Duration(0.5)) && ros::ok()) {
				if (wait == 0) {
					std::cout << "\r                              \rWaiting for new orders" << std::flush;
				}
				wait++;
				std::cout << "." << std::flush;
				if (wait ==  8) {
					std::cout << "\e[A" << std::endl;
					wait = 0;
				}
			}
			std::cout << "\n\nMoving the robot !" << std::endl;
			target_pose_link.header.stamp = t;
			tr.transformPose("world", target_pose_link, target_pose_world);
			pub.publish(target_pose_link);
			group.setPoseTarget(target_pose_world.pose, "gripper");
	// 		pub2.publish(pt);
			
			bool success = group.plan(my_plan);
			if (success) {
				group.move();
				sleep(3);
				
				// Debut attrape
				
				target_pose_link.pose.position.x = 0.13;
				target_pose_link.pose.position.y = 0;
				target_pose_link.pose.position.z = 0;
				
				q.setRPY(1.56, 3.14, 0);
				target_pose_link.pose.orientation.x = q.getX();
				target_pose_link.pose.orientation.y = q.getY();
				target_pose_link.pose.orientation.z = q.getZ();
				target_pose_link.pose.orientation.w = q.getW();
				target_pose_link.header.frame_id = "Approche";
				
				tr.transformPose("world", target_pose_link, target_pose_world);
				pub.publish(target_pose_world);
				group.setPoseTarget(target_pose_world.pose);
				
				
				success = group.plan(my_plan);
				if (success)
					group.move();
				
				
				sleep(3);
				
				// fiN ATTRAPE
					
				b.request.data = true;
				grip.call(b);
				std::cout << "Position reached !" << std::endl;
				sleep(3);
			}
			else 
				std::cout << "Position out of reach !" << std::endl;
			std::cout << "Going back to default position." << std::endl;
			target_pose_link.pose.position.x = 0.7;
			target_pose_link.pose.position.y = -1.42;
			target_pose_link.pose.position.z = 2.0;
			q.setRPY(0.054, 0.948, -1.5590);
			target_pose_link.pose.orientation.x = 3.14;
			target_pose_link.pose.orientation.y = -3.14;
			target_pose_link.pose.orientation.z = 0;
			target_pose_link.pose.orientation.w = 1.0;
			target_pose_link.header.frame_id = "world";
			group.setPoseTarget(target_pose_link.pose);
			success2 = group.plan(my_plan);
			if (success2)
				group.move();
			std_srvs::SetBool r;
// 			sleep(3);
			r.request.data = false;
			cl.call(r);
		}
		catch (tf::TransformException ex){
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
		}
		
	}
	ros::spin();
	return 0;
}