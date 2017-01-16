#ifndef _GAZEBO_CONTACT_PLUGIN_HH_
#define _GAZEBO_CONTACT_PLUGIN_HH_

#include <string>
#include <iostream>
#include <memory>

#include <ros/ros.h>
#include <geometry_msgs/Wrench.h>

#include <gazebo/gazebo.hh>
#include <gazebo/sensors/sensors.hh>
#include <gazebo_plugins/gazebo_ros_ft_sensor.h>
#include <force_sensor/Wrench_list.h>
#include <gazebo_plugins/gazebo_ros_ft_sensor.h>

  /// \brief An example plugin for a contact sensor.
  class ContactPlugin : public gazebo::SensorPlugin
  {
    /// \brief Constructor.
    public: ContactPlugin();

    /// \brief Destructor.
    public: virtual ~ContactPlugin();

    /// \brief Load the sensor plugin.
    /// \param[in] _sensor Pointer to the sensor that loaded this plugin.
    /// \param[in] _sdf SDF element that describes the plugin.
    public: virtual void Load(gazebo::sensors::SensorPtr _sensor, sdf::ElementPtr _sdf);

    /// \brief Callback that receives the contact sensor's update signal.
    private: virtual void OnUpdate();

    /// \brief Pointer to the contact sensor
    private: gazebo::sensors::ContactSensorPtr parentSensor;

    /// \brief Connection that maintains a link between the contact sensor's
    /// updated signal and the OnUpdate callback.
    private: gazebo::event::ConnectionPtr updateConnection;
	     ros::Publisher pub;
  };

#endif