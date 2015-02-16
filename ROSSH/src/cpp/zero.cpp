#include <ros/ros.h>
#include <dcc/RosDccData.h>
#include <dcc/FreqChange.h>

bool freqChange(dcc::FreqChange::Request &req, dcc::FreqChangeResponse &res){
	ROS_INFO("Heard freq change request");
	for (int i=0;i<(int)req.desired_list.size();i++){
		ROS_INFO("Desired: %d",req.desired_list[i]);
	}
	return true;
}
int main(int argc, char** argv){
	ROS_INFO("Faker is publishing %d fake frequencies on /dcc/signal",argc-1);

	if (argc<2){return EXIT_FAILURE;}
	ros::init(argc,argv,"faker");
	ros::NodeHandle nh("dcc");
	ros::ServiceServer ss = nh.advertiseService("/dcc/freq_change",&freqChange);
	ros::Publisher out = nh.advertise<dcc::RosDccData>("/dcc/signal",1,true);
	int i=1;
	ros::AsyncSpinner as(2);
	as.start();
	while(ros::ok()){
		i++;
		i=i%(argc);
		if (i==0){i++;}
		dcc::RosDccData msg;
		msg.strength=0;
		msg.frequency=atoi(argv[i]);
		ros::Duration(.6).sleep();
		out.publish(msg);
		ros::Duration(.6).sleep();
		out.publish(msg);
	}
	as.stop();
}
