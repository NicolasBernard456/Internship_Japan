cmake_minimum_required(VERSION 2.8.3)
project(force_sensor)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
	gazebo
	gazebo_msgs
	roscpp
	rospy
	std_msgs
	message_generation
)



include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND "${GAZEBO_CXX_FLAGS}")

add_message_files(
  FILES
  Wrench_list.msg
)
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES force_sensor
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
CATKIN_DEPENDS message_runtime
)

add_library(contact SHARED src/ContactPlugin.cpp)
target_link_libraries(contact ${GAZEBO_libraries} ${catkin_LIBRARIES})
add_dependencies(contact ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} ${gazebo_msgs_dep})



