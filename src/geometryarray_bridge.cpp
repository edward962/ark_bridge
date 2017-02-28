#include <ros/ros.h>
#include <ros/console.h>
#include <lcm_to_ros/GeometryArray.h>
#include <lcm_to_ros/Point.h>
#include <lcm_to_ros/Attribute.h>
#include <lcm_to_ros/GeometryFeature.h>
#include <cpr_msgs/GeometryArray.h>
#include <geometry_msgs/Point.h>
#include <autonomy_msgs/Attribute.h>
#include <cpr_msgs/GeometryFeature.h>
#include <stdlib.h>

ros::Publisher pub;
std::string lcm_topic, ros_topic, direction;

void rosCallback(const cpr_msgs::GeometryArray::ConstPtr& msg)
{
  lcm_to_ros::GeometryArray bridge_message;

  bridge_message.header.seq = msg->header.seq;
  bridge_message.header.frame_id = msg->header.frame_id;
  bridge_message.header.stamp = msg->header.stamp;

  bridge_message.nfeatures = msg->features.size()

  for(int f = 0; f < msg->features.size(); f++){
    lcm_to_ros::GeometryFeature gf;

      gf.id = msg->features[f].id;
      gf.type = msg->features[f].type;
      gf.npoints = msg->features[f].points.size();
      gf.nattributes = msg->features[f].attributes.size();
      gf.ndata = msg->features[f].data.size();

      for(int p = 0; p < msg->features[f].points.size(); p++){
        lcm_to_ros::Point[] pp;
        pp.x = msg->features[f].points[p].x;
        pp.y = msg->features[f].points[p].y;
        pp.z = msg->features[f].points[p].z;

        gf.points.push_back(pp);
      }

      for(int a = 0; a < msg->features[f].attributes.size(); a++){
        lcm_to_ros::Attribute[] aa;
        aa.name = msg->features[f].attributes[a].name;
        aa.offset = msg->features[f].attributes[a].offset;
        aa.datatype = msg->features[f].attributes[a].datatype;
        aa.length = msg->features[f].attributes[a].length;

        gf.attributes.push_back(aa);
      }

      for(int d = 0; d < msg->features[f].data.size(); d++){
        gf.data.push_back(msg->features[f].data[d]);
      }

    bridge_message.features.push_back(gf);
  }

  pub.publish(bridge_message);
}

void lcmCallback(const lcm_to_ros::GeometryArray::ConstPtr& msg)
{
  cpr_msgs::GeometryArray bridge_message;

  bridge_message.header.seq = msg->header.seq;
  bridge_message.header.frame_id = msg->header.frame_id;
  bridge_message.header.stamp = msg->header.stamp;

  bridge_message.nfeatures = msg->features.size()

  for(int f = 0; f < msg->features.size(); f++){
    cpr_msgs::GeometryFeature gf;

      gf.id = msg->features[f].id;
      gf.type = msg->features[f].type;
      gf.npoints = msg->features[f].points.size();
      gf.nattributes = msg->features[f].attributes.size();
      gf.ndata = msg->features[f].data.size();

      for(int p = 0; p < msg->features[f].points.size(); p++){
        geometry_msgs::Point[] pp;
        pp.x = msg->features[f].points[p].x;
        pp.y = msg->features[f].points[p].y;
        pp.z = msg->features[f].points[p].z;

        gf.points.push_back(pp);
      }

      for(int a = 0; a < msg->features[f].attributes.size(); a++){
        autonomy_msgs::Attribute[] aa;
        aa.name = msg->features[f].attributes[a].name;
        aa.offset = msg->features[f].attributes[a].offset;
        aa.datatype = msg->features[f].attributes[a].datatype;
        aa.length = msg->features[f].attributes[a].length;

        gf.attributes.push_back(aa);
      }

      for(int d = 0; d < msg->features[f].data.size(); d++){
        gf.data.push_back(msg->features[f].data[d]);
      }

    bridge_message.features.push_back(gf);
  }

  pub.publish(bridge_message);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "geometryarray_remapper");
  ros::NodeHandle nh("~");
  ros::Subscriber sub;

  if(nh.getParam("ros_topic", ros_topic) && nh.getParam("lcm_topic", lcm_topic) && nh.getParam("direction", direction)){
     if(!direction.compare("ros2lcm")){
       ROS_INFO("(%s) LCM <-- ROS (%s)", lcm_topic.c_str(), ros_topic.c_str());
    }
    else{
      ROS_INFO("(%s) LCM --> ROS (%s)", lcm_topic.c_str(), ros_topic.c_str());
    }

    if(!direction.compare("ros2lcm")){
      pub = nh.advertise<lcm_to_ros::GeometryArray>(lcm_topic, 10);
      sub = nh.subscribe(ros_topic, 10, rosCallback);
    }
    else{
      pub = nh.advertise<cpr_msgs::GeometryArray>(ros_topic, 10);
      sub = nh.subscribe(lcm_topic, 10, lcmCallback);
    }

    ros::spin();
  }
  else{
    ROS_ERROR("Missing Parameters!");
  }

  return 0;

}
