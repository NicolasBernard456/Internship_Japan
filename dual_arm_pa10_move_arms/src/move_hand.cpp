
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

#include <std_msgs/Float64.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>
#include <moveit/planning_interface/planning_interface.h>
#include <std_msgs/Bool.h>

#include "tf/message_filter.h"
#include <geometry_msgs/PointStamped.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <shape_msgs/SolidPrimitive.h>

#include<dual_arm_pa10_move_arms/Hand_state.h>
#include <dual_arm_pa10_move_arms/Hand_stateRequest.h>
#include <dual_arm_pa10_move_arms/Hand_stateResponse.h>

ros::Publisher pub, pub2, pub3, planning_scene_diff_publisher;
ros::ServiceClient cl;

bool cb_service(dual_arm_pa10_move_arms::Hand_stateRequest& req, dual_arm_pa10_move_arms::Hand_stateResponse& rep) {
	static moveit::planning_interface::MoveGroup group("gripper_right");
	static moveit::planning_interface::MoveGroup::Plan my_plan;
	group.setPlannerId("RRTConnectkConfigDefault");
	group.setGoalOrientationTolerance(0.0001);
	group.setGoalPositionTolerance(0.0001);
	group.setGoalJointTolerance(0.0001);
	if (strcmp(req.state.c_str(), "default_hand") == 0)
		group.setNamedTarget("default_hand");
	else if (strcmp(req.state.c_str(), "grasp") == 0)
		group.setNamedTarget("grasp");
	else if (strcmp(req.state.c_str(), "prehasping") == 0)
		group.setNamedTarget("prehasping");
	else{
		ROS_ERROR("Group state unknown,  no mouvement executed");
		ROS_INFO("Try \"default_hand\" or \"prehasping\" " );
// 		std::cout <<  req.state << std::endl;
		return false;
	}
	if (group.plan(my_plan)) {
		group.move();
		rep.result = true;
		return true;
	}
	else{
		rep.result = false;
		return false;
	}
}

int main(int argc, char **argv){
	ros::init (argc, argv, "move_hand");
	ros::AsyncSpinner spinner(1);
	spinner.start();
	ros::NodeHandle n;
	
	pub = n.advertise<geometry_msgs::PoseStamped>("/Pose_trajectory", 1);
	pub2 = n.advertise<geometry_msgs::PoseStamped>("/Pose_trajectory_origin", 1);

	ros::ServiceServer srv = n.advertiseService("/Control_hand", cb_service);
	
	ros::spin();
	
	
	return 0;
}