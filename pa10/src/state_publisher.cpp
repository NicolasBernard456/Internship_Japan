#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "state_publisher");
    ros::NodeHandle n;
    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    tf::TransformBroadcaster broadcaster;
    ros::Rate loop_rate(30);

    const double degree = M_PI/180;

    // robot state
    double tilt = 0, tinc = degree, swivel=0, angle=0, height=0, hinc=0.005;

    // message declarations
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "link1";

    while (ros::ok()) {
        //update joint_state
// 	ROS_INFO("OK");
        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(7);
        joint_state.position.resize(7);
        
	joint_state.name[0] ="joint1";
        joint_state.position[0] = swivel;
	joint_state.name[1] ="joint2";
        joint_state.position[1] = 0;
	joint_state.name[2] ="joint3";
        joint_state.position[2] = 0;
	joint_state.name[3] ="joint4";
        joint_state.position[3] = 0;
	joint_state.name[4] ="joint5";
        joint_state.position[4] = 0;
	joint_state.name[5] ="joint6";
        joint_state.position[5] = 0;
	joint_state.name[6] ="joint7";
        joint_state.position[6] = 0;
	



        // update transform
        // (moving in a circle with radius=2)
        odom_trans.header.stamp = ros::Time::now();
        odom_trans.transform.translation.x = 0;
        odom_trans.transform.translation.y = 1;
        odom_trans.transform.translation.z = 0;
        odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(M_PI/2);

        //send the joint state and transform
        joint_pub.publish(joint_state);
        broadcaster.sendTransform(odom_trans);

// 	ROS_INFO("OK2");
        // Create new robot state
        tilt += tinc;
        if (tilt<-.5 || tilt>0) tinc *= -1;
        height += hinc;
        if (height>.2 || height<0) hinc *= -1;
        swivel += degree;
        angle += degree/4;
// 	ROS_INFO("OK3");
        // This will adjust as needed per iteration
        loop_rate.sleep();
// 	ROS_INFO("OK4");
	ros::spinOnce();
    }


    return 0;
}
