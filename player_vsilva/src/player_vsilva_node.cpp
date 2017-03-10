//#include <iostream>

//int main()
//{
//    std::cout << "Hello world" << std::endl;           
//    return 1;                                                      
//}

#include <iostream>  
#include <vector> 
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>

                         
using namespace std; 
                                                         
namespace rwsua2017
{


class MyPlayer:public Player
{
    public: 

    MyPlayer(string argin_name, string argin_team_name): Player(argin_name,argin_team_name)
    {
        //setTeamName(team);
	cout << "MyPlayer" << endl;
    }

	vector<string> teammates;

};



}



int main()
{

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

	return 1;

}
