#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <future>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstdlib>

//Temporary classes
enum Direction { none, up, down, left, right };


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}
	~StudentWorld() {
		deallIce();
		deallIceMan();
	}

	virtual int init()
	{
		//Create the Ice Field
		createField();
		//Place the player
		placePlayer();
		//Place the barrel
		placeBarrels(this->getLevel());
		//Place Gold
		placeGold(this->getLevel());
		//Place Boulders
		placeBoulders(this->getLevel());
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		if (!Player->isAlive())
			return GWSTATUS_PLAYER_DIED;
		if (Barrels.size() == 0)
			return GWSTATUS_FINISHED_LEVEL;
		//Display score
		setDisplayText();
		//Place Sonar
		placeSonar();
		//Place Puddles
		placePuddle();
		//Get player controls
		int PlayerMovement;
		//If anything else is pressed
		if (getKey(PlayerMovement) == true) {
			//If ESC is pressed

			if (PlayerMovement == KEY_PRESS_ESCAPE)
			{
				decLives();
				playSound(SOUND_PLAYER_GIVE_UP);
				return GWSTATUS_PLAYER_DIED;
			}
			else //Otherwise do stuff
				Player->doSomething(PlayerMovement);
		}

		isThereIce(Player->getX(), Player->getY());//Delete the ice as the player moves
		BarrelsDo();//Make Barrels do their thing
		GoldDo();//Make Gold do stuff
		BoulderMain();//Boulders do stuff, sort of...
		PuddleDo();
		SonarDo();//Sonar doing stuff....
		if (Spurts)
			Spurts->doSomething();
		cleanDeadStuff();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		cleanEVERYTHING();
	}
	//Misc Public Functions
	//////////////////////////
	/////////////////////////
	////////////////////////
	////////////////////////
	bool checkForBoulders(int x, int y);
	
	// !!! FixMe !!! RAV for Player
	double distanceFunc(const actor* a,const actor* b) 
	{
		double x = a->getX() - b->getX();
		double y = a->getY() - b->getY();
		return sqrt(pow(x,2) + pow(y,2));
	}
	double distanceFuncUnique(int Ax,int Ay, int Bx, int By)
	{
		double x = Ax - Bx;
		double y = Ay - By;
		return sqrt(pow(x,2) + pow(y,2));
	}
	double distanceFunc(const actor* a) // check distance from player 
	{
		double x = a->getX() - Player->getX(); // read access violation?
		double y = a->getY() - Player->getY(); // why is Player value unknown
		return sqrt(pow(x,2) + pow(y,2));
	}
	//Player Helper functions
	void incrementGold() { Player->incrementGoldCount(); }
	void incrementSonar() { Player->incrementSonar(); }
	void incrementSquirt() { Player->incrementSquirtGun(); }
	void goldDrop();
	void SquirtGunFire();
	bool squirtTraversal(int sDir);
	void SonarAction();
private:
	//Container for coordinates
	struct coor {
	private:
		int X;
		int Y;
	public:
		coor(int x, int y) { X = x; Y = y; }
		int getX() { return X; }
		int getY() { return Y; }
	};
	//Pointer for spurts
	std::unique_ptr<WaterSpurt> Spurts;
	//Pointer for Sonar
	std::unique_ptr<SonarKit> SonarItem;
	//Pointer for puddle
	std::vector<std::unique_ptr<WaterPool>> Puddle;
	//Ice container 2D unique pointer array
	std::unique_ptr<Ice> iceMap[64][64];
	
	//Unique pointer for player
	std::unique_ptr<Iceman> Player;
	
	//Keep track of Items on the field
	std::vector <std::unique_ptr<Gold>> Golds; // mabey an array of each pointer to an
	std::vector <std::unique_ptr<Gold>> dGolds;
	std::vector <std::unique_ptr<Barrel>> Barrels;
	std::vector <std::unique_ptr<Boulder>> Boulders;
	std::vector <std::unique_ptr<coor>> masterList;
	//Protesters
	std::vector < std::unique_ptr<Protester>> Protesters;
	//Helper Functions for field
	void createField();
	void placeBarrels(int lvl);
	void placePlayer();
	bool isThereIce(int x, int y);
	void Randomizer(int &x, int &y, int xRng, int yRng, int xDom, int yDom);
	void BarrelsDo();
	bool checkIfTooClose(int Ax, int Ay, int Bx, int By);
	int probabilityCheck();
	void placeSonar();
	void placePuddle();
	void killPlayer() { Player->setAlive(false); playSound(SOUND_PLAYER_GIVE_UP); }
	//Gold helpers
	void placeGold(int lvl);
	void GoldDo();
	//Boulder Helpers
	void BoulderMain();
	bool BoulderIceCheck(int x, int y);
	void placeBoulders(int lvl);
	//Helper Functions for the cleanup
	void deallIce();
	void deallIceMan();
	void deallBarrels();
	void deallGold();
	void deallBoulders();
	void deallSonar();
	void deallPuddle();
	void cleanDeadStuff();
	void cleanEVERYTHING();
	//Sonar Helper Functions
	void SonarDo() { if(SonarItem)SonarItem->doSomething(); }
	void decrementSonar() { Player->decrementSonar(); }
	//Puddle Helper Functions
	void findPuddleHome(int &x, int &y);
	void PuddleDo();
	bool checkPuddleArea(const int &x, const int &y);
	//Display text helper function
	void setDisplayText();
	//Squirt Helper Functions
	void decrementSquirt() { Player->decrementSquirtGun(); }
	
	

};

#endif // STUDENTWORLD_H_