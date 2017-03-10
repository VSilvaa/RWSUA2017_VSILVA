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

                         
using namespace std;
using namespace boost;
                                                         
namespace rwsua2017
{


	class MyPlayer:public Player
	{
		ros::Subscriber sub;
	    public: 
		


	    MyPlayer(string argin_name, string argin_team_name): Player(argin_name,argin_team_name)
	    {

		sub = n.subscribe("/make_a_play", 1000, &MyPlayer::make_plays_callback,this);

		//setTeamName(team);
		cout << "MyPlayer" << endl;
	    }

		vector<string> teammates;

	void make_plays_callback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
	{
	  cout << "Coiso!! with max disp.= "<< msg->max_displacement << endl;
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

	myplayer.teammates.push_back("vsilva");
	myplayer.teammates.push_back("fsilva");


	for(size_t i=0;i<myplayer.teammates.size();++i)
	{
		cout << myplayer.teammates[i] << endl;
	}
	
	ros::spin();

	return 1;

}
