/**
Software License Agreement (BSD)

\file      listmaps_service_caller.cpp
\authors   Dave Niewinski <dniewinski@clearpathrobotics.com>
\copyright Copyright (c) 2017, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the 
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <ros/ros.h>
#include <ros/console.h>
#include <ark_bridge/StringArray.h>
#include <ark_bridge/Empty.h>
#include <map_data_msgs/ListMaps.h>
#include <stdlib.h>

ros::Publisher pub;
ros::ServiceClient serv;
std::string call_topic, response_topic, service_name;

void rosCallback(const ark_bridge::Empty::ConstPtr& msg)
{
  map_data_msgs::ListMaps srv;

  if(serv.call(srv)){
    ark_bridge::StringArray response_message;
    response_message.data = srv.response.maps;
    response_message.nstrings = srv.response.maps.size();
    pub.publish(response_message);
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "listmaps_service_caller");
  ros::NodeHandle nh("~");
  ros::Subscriber sub;

  if(nh.getParam("call_topic", call_topic) && nh.getParam("service_name", service_name) && nh.getParam("response_topic", response_topic)){
    ROS_INFO("(%s) --> <%s> --> (%s)", call_topic.c_str(), service_name.c_str(), response_topic.c_str());

    pub = nh.advertise<ark_bridge::StringArray>(response_topic, 1, true);
    sub = nh.subscribe(call_topic, 10, rosCallback);
    serv = nh.serviceClient<map_data_msgs::ListMaps>(service_name);
    ros::spin();
  }
  else{
    ROS_ERROR("Missing Parameters!");
  }

  return 0;
}
