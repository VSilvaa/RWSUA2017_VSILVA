//#include <iostream>

//int main()
//{
//    std::cout << "Hello world" << std::endl;           
//    return 1;                                                      
//}

#include <iostream>  
#include <vector> 
                         
using namespace std; 
                                                         
namespace rwsua2017
{

class Player
{
    public:

    Player(string argin_name, string argin_team_name="blue") //Mesmo nome porque é construtora, corre quando se chama ~
    {
        this->name = name;
	set_team_name(argin_team_name);
    }


    string name;

	//accessor
	void set_team_name(string argin_team_name)
	{
		if( argin_team_name=="red" || argin_team_name=="blue" || argin_team_name=="green" )
		{
			this->team_name = argin_team_name;
		}
		else 
		{
			cout << "ERRO no nome!" << endl;
		}
	}

	void set_team_name(void)
	{
		set_team_name("red");
	}

	string get_team_name(void)	
	{
		return this->team_name;
	}

    private:

	string team_name;



};



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
