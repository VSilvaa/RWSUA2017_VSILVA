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

	    public: 
		


	    MyPlayer(string argin_name, string argin_team_name): Player(argin_name,argin_team_name)
	    {

		sub = n.subscribe("/make_a_play/cat", 1000, &MyPlayer::make_plays_callback,this);

		  t1.setOrigin( tf::Vector3(randNumber(), randNumber(), 0.0) );
		  tf::Quaternion q;
		  q.setRPY(0, 0, 0);
		  t1.setRotation(q);
		  br.sendTransform(tf::StampedTransform(t1, ros::Time::now(), "map",name));


		//setTeamName(team);
		cout << "MyPlayer" << endl;
	    }

		vector<string> teammates;



	float getAnleto(string player_name)
	{
		tf::StampedTransform transf;

	    try{
	      listener.waitForTransform(name,player_name,ros::Time(0), Duration(0.1));
	      listener.lookupTransform(name, player_name, ros::Time(0), transf);
	    }
	    catch (tf::TransformException ex){
	      ROS_ERROR("%s",ex.what());
	      ros::Duration(1.0).sleep();
	    }

	    float ang =  atan2(transf.getOrigin().y(),transf.getOrigin().x());

	return ang;

	}

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



	void make_plays_callback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
	{
	  cout << "Coiso!! with max disp.= "<< msg->max_displacement << endl;

	  Transform t_mov;

	  float turn_angle=getAnleto("moliveira");
	  float displacement=msg->max_displacement;
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
