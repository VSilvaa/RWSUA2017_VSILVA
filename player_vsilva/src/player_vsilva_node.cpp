//#include <iostream>

//int main()
//{
//    std::cout << "Hello world" << std::endl;           
//    return 1;                                                      
//}

#include <iostream>  
#include <vector> 
#include <ros/ros.h>
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <visualization_msgs/Marker.h>

#define MAX_ANGLE M_PI/25

                         
using namespace std;
using namespace boost;
using namespace tf;
using namespace ros;

double randNumber(){
	struct timeval t1;
	gettimeofday(&t1,NULL);
	srand(t1.tv_usec);
	double x =((((double)rand()/(double)RAND_MAX)*2 -1)*5);

	return x;
}

                                                         
namespace rwsua2017
{


	class MyPlayer:public Player
	{
		ros::Subscriber sub;
		tf::TransformBroadcaster br;
		tf::TransformListener listener;
		tf::Transform t1;
		Publisher vis_pub;

	    public: 
		

//---------------------------------------------------------------------------------------
	    MyPlayer(string argin_name, string argin_team_name): Player(argin_name,argin_team_name)
	    {

		sub = n.subscribe("/make_a_play/cat", 1000, &MyPlayer::make_plays_callback,this);
                vis_pub = n.advertise<visualization_msgs::Marker>( "/bocas", 0 );

		  t1.setOrigin( tf::Vector3(randNumber(), randNumber(), 0.0) );
		  tf::Quaternion q;
		  q.setRPY(0, 0, 0);
		  t1.setRotation(q);
		  br.sendTransform(tf::StampedTransform(t1, ros::Time::now(), "map",name));


		//setTeamName(team);
		cout << "MyPlayer" << endl;
	    }

		vector<string> teammates;


//---------------------------------------------------------------------------------------
float getAngleTo(string player_name)
	   {
		tf::StampedTransform trans;
		ros::Time now = Time(0); //get the latest transform received
		try{
			listener.waitForTransform(name,player_name,now,Duration(0.1));
			listener.lookupTransform(name, player_name,now, trans);
		}
		catch (tf::TransformException &ex) {
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
		}
		float x = trans.getOrigin().x();
		float y = trans.getOrigin().y();
		double angle = atan2(y,x);
	   }
//---------------------------------------------------------------------------------------
	tf::StampedTransform getPose(void)
	{
	    tf::StampedTransform transf;
	    try{
	      listener.waitForTransform("map",name,ros::Time(0), Duration(0.1));
	      listener.lookupTransform("map", name, ros::Time(0), transf);
	    }
	    catch (tf::TransformException ex){
	      ROS_ERROR("%s",ex.what());
	      ros::Duration(1.0).sleep();
	    }

	    return transf;
	}

//---------------------------------------------------------------------------------------
double getDistFromTo(string from, string to){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	float time_to_wait = 0.1;
	double dist = 0;
	 try{
	listener.waitForTransform(from,to,now, ros::Duration(time_to_wait));
	listener.lookupTransform(from, to,
															now,transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
    }


			double x = transform.getOrigin().x();
        double y = transform.getOrigin().y();
        dist = sqrt(x*x + y*y);

	return dist;
}

//---------------------------------------------------------------------------------------
	void make_plays_callback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
	{	  
		cout << "I received a MakeAPlay message" << endl;
		cout << "max_displacement = " << msg->max_displacement << endl;

		// Definição dos angulos de rotação e valores de translação


		float displacement=msg->max_displacement;


			double dist[3];

			dist[0] = getDistFromTo(name, "rmartins");
			dist[1] = getDistFromTo(name, "jferreira");
			dist[2] = getDistFromTo(name, "fsilva");

			int safedist = 2;
			double angleC;
			if(dist[0] < safedist || dist[1] < safedist || dist[2] < safedist){
				if(dist[0] < safedist){
					angleC = -getAngleTo("rmartins");
				}else if(dist[1] < safedist){
					angleC = -getAngleTo("jferreira");
				}else{
						angleC = -getAngleTo("fsilva");
				}
			}else{
				angleC = getAngleTo("moliveira");
			}

			move(displacement, angleC, MAX_ANGLE);


			visualization_msgs::Marker marker;
			marker.header.frame_id = name;
			marker.header.stamp = ros::Time();
			marker.ns = name;
			marker.id = 0;
			marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
			marker.action = visualization_msgs::Marker::ADD;
			marker.pose.position.x = 0;
			marker.pose.position.y = 0.4;
			marker.pose.position.z = 0;
			marker.pose.orientation.x = 0.0;
			marker.pose.orientation.y = 0.0;
			marker.pose.orientation.z = 0.0;
			marker.pose.orientation.w = 1.0;
			marker.scale.x = 1;
			marker.scale.y = 0.1;
			marker.scale.z = 0.4;
			marker.color.a = 1.0; // Don't forget to set the alpha!
			marker.color.r = 0.3;
			marker.color.g = 0.3;
			marker.color.b = 0.3;
			marker.text="Vou-te Comer!";
			marker.frame_locked=1;
			marker.lifetime=ros::Duration(1);
			//only if using a MESH_RESOURCE marker type:
			marker.mesh_resource = "package://pr2_description/meshes/base_v0/base.dae";
			vis_pub.publish( marker );




	}

//---------------------------------------------------------------------------------------	
	void move(float displacement , float turn_angle , float max_turn_angle)
	{
          Transform t_mov;

	  if(turn_angle > max_turn_angle) turn_angle= max_turn_angle;
	  else if(turn_angle < -max_turn_angle) turn_angle= -max_turn_angle;

	  TransformListener listener;

	  t1=getPose();

	  
	  t_mov.setOrigin( tf::Vector3(displacement, 0, 0.0) );
	  Quaternion q;
	  q.setRPY(0, 0, turn_angle);
	  t_mov.setRotation(q);
	  Transform t=t1*t_mov;
	  br.sendTransform(tf::StampedTransform(t, ros::Time::now(), "map",name));

	}


	};

}
//---------------------------------------------------------------------------------------
int main(int argc, char **argv)
{

	ros::init(argc, argv, "listener");

    string player_name = "vsilva";
    //Creating an instance of class Player
    rwsua2017::MyPlayer myplayer(player_name,"green");

	cout << "Created an instance of class player with public name " << myplayer.name << endl;
	cout << "Team_name = " << myplayer.get_team_name() << endl;

	myplayer.teammates.push_back("dcorreia");
	myplayer.teammates.push_back("jsousa");


	for(size_t i=0;i<myplayer.teammates.size();++i)
	{
		//cout << myplayer.teammates[i] << endl;
	}
	
	ros::spin();

	return 1;

}
