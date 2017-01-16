#include "ContactPlugin.hh"

using namespace gazebo;
GZ_REGISTER_SENSOR_PLUGIN(ContactPlugin)

/////////////////////////////////////////////////
ContactPlugin::ContactPlugin() : SensorPlugin(){
	char* argv[1];
	int argc = 0;
	ros::init(argc, argv,"Contactplugin");
	ros::NodeHandle n;
	pub = n.advertise<force_sensor::Wrench_list>("dual_arm_pa10/Wrench", 1);
}

/////////////////////////////////////////////////
ContactPlugin::~ContactPlugin()
{
}

/////////////////////////////////////////////////
void ContactPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr /*_sdf*/)
{
	
  // Get the parent sensor.
  this->parentSensor = boost::dynamic_pointer_cast<sensors::ContactSensor>(_sensor);
  // Make sure the parent sensor is valid.
  if (!this->parentSensor)
  {
    gzerr << "ContactPlugin requires a ContactSensor.\n";
    return;
  }

  // Connect to the sensor update event.
  this->updateConnection = this->parentSensor->ConnectUpdated(
      boost::bind(&ContactPlugin::OnUpdate, this));

  // Make sure the parent sensor is active.
  this->parentSensor->SetActive(true);
}

/////////////////////////////////////////////////
void ContactPlugin::OnUpdate(){
	// Get all the contacts.
	msgs::Contacts contacts;
	contacts = this->parentSensor->GetContacts();
	geometry_msgs::Wrench w;
	force_sensor::Wrench_list w_list;
	for (unsigned int i = 0; i < contacts.contact_size(); ++i){
// 		std::cout << "Collision between[" << contacts.contact(i).collision1()
// 			<< "] and [" << contacts.contact(i).collision2() << "]\n";
		int k = 0;
		for (unsigned int j = 0; j < contacts.contact(i).position_size(); ++j){
			// FILTRAGE
			if (fabs(contacts.contact(i).wrench(j).body_1_wrench().force().x()) > pow(10, -10) && fabs(contacts.contact(i).wrench(j).body_1_wrench().force().y()) > pow(10, -10) && fabs(contacts.contact(i).wrench(j).body_1_wrench().force().z()) > pow(10, -10)) {
// 				std::cout << k << "  Position:"
// 						<< contacts.contact(i).position(j).x() << " "
// 						<< contacts.contact(i).position(j).y() << " "
// 						<< contacts.contact(i).position(j).z() << "\n";
// 				std::cout << "   Normal:"
// 						<< contacts.contact(i).normal(j).x() << " "
// 						<< contacts.contact(i).normal(j).y() << " "
// 						<< contacts.contact(i).normal(j).z() << "\n";
// 				std::cout << "  Force:" 
// 						<< contacts.contact(i).wrench(j).body_1_wrench().force().x() << " "
// 						<< contacts.contact(i).wrench(j).body_1_wrench().force().y() << " "
// 						<< contacts.contact(i).wrench(j).body_1_wrench().force().z() << "\n";
// 				std::cout << "   Moment:" 
// 						<< contacts.contact(i).wrench(j).body_1_wrench().torque().x() << " "
// 						<< contacts.contact(i).wrench(j).body_1_wrench().torque().y() << " "
// 						<< contacts.contact(i).wrench(j).body_1_wrench().torque().z() << "\n";
				// Add wrench message to a list of wrench message
				w.force.x = contacts.contact(i).wrench(j).body_1_wrench().force().x();
				w.force.y = contacts.contact(i).wrench(j).body_1_wrench().force().y();
				w.force.z = contacts.contact(i).wrench(j).body_1_wrench().force().z();
				w.torque.x = contacts.contact(i).wrench(j).body_1_wrench().torque().x();
				w.torque.y = contacts.contact(i).wrench(j).body_1_wrench().torque().y();
				w.torque.z = contacts.contact(i).wrench(j).body_1_wrench().torque().z();
				w_list.wrench_list.push_back(w);
				w_list.name_col1.push_back(contacts.contact(i).collision1());
				w_list.name_col2.push_back(contacts.contact(i).collision2());
				k++;
			}
			pub.publish(w_list);
		}
	}
}