#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/Float64.h>
#include <std_srvs/SetBoolRequest.h>
#include <std_srvs/SetBoolResponse.h>

ros::Publisher pub_finger_left, pub_finger_right;

bool control(std_srvs::SetBool::Request& req, std_srvs::SetBool::Response& resp) {
	if (req.data == true) {                                      //Close gripper
		std_msgs::Float64 left, right;
		left.data = -10000.0;
		right.data = 10000.0;
		pub_finger_left.publish(left);
		pub_finger_right.publish(right);
		resp.message = "Gripper closed";
		resp.success = true;
		return true;
	}
	else{ 
	 	std_msgs::Float64 left, right;
		left.data = 10000.0;
		right.data = -10000.0;
		pub_finger_left.publish(left);
		pub_finger_right.publish(right);
		resp.message = "Gripper open";
		resp.success = true;
		return true;
	}
		
}



int main(int argc,  char* argv[]) {
	ros::init(argc, argv, "gripper_control");
	ros::NodeHandle n;
	ros::ServiceServer service = n.advertiseService("/Gripper_control", control);	
	
	pub_finger_left = n.advertise<std_msgs::Float64>("/pa10/gripper_controller/joints/left_prism/command", 1);
	pub_finger_right = n.advertise<std_msgs::Float64>("/pa10/gripper_controller/joints/right_prism/command", 1);
	
	ros::spin();
	
	return 0;
}