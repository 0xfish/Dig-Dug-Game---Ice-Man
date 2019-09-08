#include "StudentWorld.h"
#include <string>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
//Sonar doing its stuff
void StudentWorld::SonarAction() {
	if (Player->getPlayerSonarChargeCount() == 0)
		return;
	playSound(SOUND_SONAR);
	decrementSonar();
	for (int i = 0; i < Barrels.size(); i++) {
		int radius = distanceFuncUnique(Player->getX(), Player->getY(), Barrels[i]->getX(), Barrels[i]->getY());
		if (radius <= 12)
			Barrels[i]->setVisible(true);
	}
	for (int i = 0; i < Golds.size(); i++) {
		int radius = distanceFuncUnique(Player->getX(), Player->getY(), Golds[i]->getX(), Golds[i]->getY());
		if (radius <= 12)
			Golds[i]->setVisible(true);
	}
}

//Drop gold
void StudentWorld::goldDrop() {
	if (Player->getGoldCount() != 0) {
		dGolds.push_back(make_unique<Gold>(Player->getX(), Player->getY()));
		dGolds.back()->setWorld(this);
		dGolds.back()->setPerm(false);
		dGolds.back()->setVisible(true);
		Player->decrementGold();
	}

}
//Make the squirt travel until it hits something
bool StudentWorld::squirtTraversal(int sDir) {
	int i = 1;
	
	switch (sDir) {
	case(1):
		if (!iceMap[Spurts->getY() + 3][Spurts->getX()]->isVisible())
			return true;
		else
			return false;
	case(2):
		if (!iceMap[Spurts->getY() - 1][Spurts->getX()]->isVisible())
			return true;
		else
			return false;
	case(3):
		if (!iceMap[Spurts->getY()][Spurts->getX() - 1]->isVisible())
			return true;
		else
			return false;
	case(4):
		if (!iceMap[Spurts->getY()][Spurts->getX() + 3]->isVisible())
			return true;
		else
			return false;
	}
}
//Special Player buttons
void StudentWorld::SquirtGunFire() {
	//Get Player direction first
	//First create squirt object oriented the same as the player
	if (!Spurts) {
		if (Player->getSquirtsLeft() != 0) {
			int sDir = Player->getPlayerDir();
			switch (sDir) {
			case(1):
				Spurts = make_unique<WaterSpurt>(Player->getX(), Player->getY() + 4, sDir);
				Spurts->setWorld(this);
				playSound(SOUND_PLAYER_SQUIRT);
				decrementSquirt();
				break;
			case(2):
				Spurts = make_unique<WaterSpurt>(Player->getX(), Player->getY() - 4, sDir);
				Spurts->setWorld(this);
				playSound(SOUND_PLAYER_SQUIRT);
				decrementSquirt();
				break;
			case(3):
				Spurts = make_unique<WaterSpurt>(Player->getX() - 4, Player->getY(), sDir);
				Spurts->setWorld(this);
				playSound(SOUND_PLAYER_SQUIRT);
				decrementSquirt();
				break;
			case(4):
				Spurts = make_unique<WaterSpurt>(Player->getX() + 4, Player->getY(), sDir);
				Spurts->setWorld(this);
				playSound(SOUND_PLAYER_SQUIRT);
				decrementSquirt();
				break;
			}
		}
	}

}
//Check for boulders to let the player know
bool StudentWorld::checkForBoulders(int x, int y) {
	for (int k = 0; k < Boulders.size(); k++) {
	//Radius of 3 is not allowed
		int radius = distanceFuncUnique(x,y,Boulders[k]->getX(),Boulders[k]->getY());
		if (radius <= 3)
			return false;
		
	}
	return true;
}



//Probability calculations for adding goodies
int StudentWorld::probabilityCheck() {
	int G = getLevel() * 25 + 300;
//Calculate G then randomly generate a number between 1 and 5,000,000
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> Range(1, G);
	std::uniform_int_distribution<std::mt19937::result_type> Range2(1, 5);
	int num = Range(rng);
	int num2 = Range2(rng);
	//Return 1 if 1/5 or return 2 if 4/5
	if (num % G == 0)
	{
		if (num2 % 5 == 0)
			return 1;
		else
			return 2;
	}
}
//Function to make sure placed objects arent too close
bool StudentWorld::checkIfTooClose(int Ax,int Ay, int Bx, int By) {
	double radius = distanceFuncUnique(Ax,Ay,Bx,By);
	if (6 <= radius)
		return false;
	else
		return true;
}
//function to display text on screen
void StudentWorld::setDisplayText() {
	int level = getLevel();
	int lives = getLives();
	int health = Player->getHealth();
	int squirts = Player->getSquirtsLeft();
	int gold = Player->getPlayerGoldCount();
	int barrels = Barrels.size();
	int sonar = Player->getPlayerSonarChargeCount();
	int score = getScore();
	//String for statistics
	ostringstream buff;
	buff << "Lvl: " << std::left << std::setw(2) << level
		<< " Lives: " << std::left << std::setw(2) << lives
		<< " Hlth: " << std::left << std::setw(3) << std::to_string(health * 10) + "% "
		<< " Wtr: " << std::left << std::setw(2) << squirts
		<< " Gld: " << std::left << std::setw(2) << gold
		<< " Oil Left: " << std::left << std::setw(2) << barrels
		<< " Sonar: " << std::left << std::setw(2) << sonar
		<< " Scr: " << std::left << std::internal << std::setfill('0') << std::setw(6) << score;
	string text = buff.str();
	setGameStatText(text);

}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
void StudentWorld::createField() {
	
	
	//Iterate to create the whole field
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			iceMap[y][x] = std::make_unique<Ice>(x, y);
		}
	}//Create the Tunnel
	for (int y = 4; y <= 59; y++) {
		for (int x = 30; x <= 33; x++) {
			iceMap[y][x]->setVisible(false);
		}
	}
	//Create top tunnel
	for (int y = 60; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			iceMap[y][x]->setVisible(false);
		}
	}

}
void StudentWorld::placePlayer() {
	Player = std::make_unique<Iceman>(30, 60);
	Player->setWorld(this);
}
//Checks for Ice
bool StudentWorld::isThereIce(int x, int y) {
	//Check if pointer is empty
	//if ((y + 4) <= 60 && (x + 3) <=63) {
		//Area is within bounds
		//Delete Ice within area
	for (int i = 0; i < 4; i++) {//X values
		for (int e = 0; e < 4; e++) { //Y values
			if ((y + i) < 64 && (x + e) < 64) {

				if (iceMap[y + i][x + e]->isVisible()) {   //If there is Ice in his area delete it
					iceMap[y + i][x + e]->setVisible(false);
					playSound(SOUND_DIG);
				}
			}
			else
				return false;
		}
	}
	return true;//If there is ice to delete return true
//}
	return false;
}
//Helper functions for cleanup
void StudentWorld::deallIce() {
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			iceMap[y][x].reset();
		}
	}
}
void StudentWorld::cleanDeadStuff() {
	//Clear out Boulders
	for (int i = 0; i < Boulders.size(); i++) {
		if (Boulders[i] && !Boulders[i]->isAlive())
			Boulders.erase(Boulders.begin() + i);
}
	
	//Clear out dead Golds
	for (int i = 0; i < Golds.size(); i++) {
		if (Golds[i] && !Golds[i]->isAlive())
			Golds.erase(Golds.begin() + i);
	}
	//Clear out Barrels
	for (int i = 0; i < Barrels.size(); i++) {
		if (Barrels[i] && !Barrels[i]->isAlive())
			Barrels.erase(Barrels.begin() + i);
	
	}
	//Clear out Sonar
		if (SonarItem && !SonarItem->isAlive())
			SonarItem.reset();
	//Clear out Puddles
		for (int i = 0; i < Puddle.size(); i++) {
			if (Puddle[i] && !Puddle[i]->isAlive())
				Puddle.erase(Puddle.begin() + i);
		}
	//Clear out gold
		for (int i = 0; i < dGolds.size(); i++) {
			if (dGolds[i] && !dGolds[i]->isAlive())
				dGolds.erase(dGolds.begin() + i);
		}
	//Clear out spurts
		if (Spurts && !Spurts->isAlive())
			Spurts.reset();

}
void StudentWorld::deallIceMan() {
	Player.reset();
}
void StudentWorld::deallBarrels()
{
	Barrels.clear();
}
void StudentWorld::deallGold()
{
	Golds.clear();
	dGolds.clear();
}
void StudentWorld::deallBoulders() {
	Boulders.clear();
}
void StudentWorld::deallSonar() {
	SonarItem.reset();
}
void StudentWorld::deallPuddle() {
	Puddle.clear();
}
void StudentWorld::cleanEVERYTHING() {
	 deallIce();
	 deallIceMan();
	 deallBarrels();
	 deallGold();
	 deallBoulders();
	 deallSonar();
	 deallPuddle();
}
void StudentWorld::placeGold(int lvl) {
	int max = 5 - ( lvl / 2 ); //Get max gold nuggets
	for (int i = 0; i < max; i++) { //Place them randomly in the field
		int x, y;
		Randomizer(x, y,60,56,0,0);
		
		//Put the new object in the master list then check to see if its at a radius of 6
		
		for (int e = 0; e < masterList.size(); e++) {
			if (checkIfTooClose(masterList[e]->getX(), masterList[e]->getY(), x, y) 
				|| 8 <= y && y <= 64 && 26 <= x && x <= 33)
			{
				while (checkIfTooClose(masterList[e]->getX(), masterList[e]->getY(), x, y)
					|| 8 <= y && y <= 64 && 26 <= x && x <= 33)
				{
					Randomizer(x, y, 60, 56, 0, 0);
				}
				//After finding the correct coordinates then place
			}
		}
		Golds.push_back(make_unique<Gold>(x, y));
		Golds[i]->setWorld(this);
		masterList.push_back(make_unique<coor>(x, y));
	}
}
void StudentWorld::placeSonar() {
	int result = probabilityCheck();
	if (SonarItem)
		return;
	if (result == 1) {
		SonarItem = make_unique<SonarKit>(0, 60, getLevel());
		SonarItem->setWorld(this);
	}
	else
		return;
}
void StudentWorld::PuddleDo() {
	for (int i = 0; i < Puddle.size(); i++)
		Puddle[i]->doSomething();
}
void StudentWorld::placePuddle() {
	int result = probabilityCheck();
	
	if (result == 2) {
		int x, y;
		findPuddleHome(x, y);
		Puddle.push_back(make_unique<WaterPool>(x, y, getLevel()));
		Puddle.back()->setWorld(this);
	}
	else
		return;
}
bool StudentWorld::checkPuddleArea(const int &x, const int &y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if ((y + i) >= 64 || (x + j) >= 64)
				return false;
			if (iceMap[y + i][x + j]->isVisible()) {
				return false;
			}
		}
	}
	return true;
}
void StudentWorld::findPuddleHome(int &x, int &y) {
	//Container to store empty ice spots
	std::vector<std::pair<int, int>> spots;

	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			if (iceMap[y][x] && !iceMap[y][x]->isVisible() && checkPuddleArea(x,y)) {
				spots.push_back(make_pair(x,y));
			}
		}
	}
//randomly choose the spots and return
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> Range(0, spots.size());
	int num = Range(rng);
	x = spots[num].first;
	y = spots[num].second;

}
void StudentWorld::placeBarrels(int lvl) { // todo: place amount of barrels based on lvl
	//!!! first place and test barrels !!!
	
	int L = 2 + lvl; // number of barrels
	if (L > 21) {
		L = 21;
	}
	// place L barrels within ice map
	for (int i = 0; i < L; i++) {
		int x, y;
		Randomizer(x, y, 60, 56,0,0);
		
		//Put the new object in the master list then check to see if its at a radius of 6
		
		for (int e = 0; e < masterList.size(); e++) {
			if (checkIfTooClose(masterList[e]->getX(), masterList[e]->getY(), x, y)
				|| 8 <= y && y <= 64 && 26 <= x && x <= 33)
			{
				while (checkIfTooClose(masterList[e]->getX(), masterList[e]->getY(), x, y)
					|| 8 <= y && y <= 64 && 26 <= x && x <= 33)
				{
					Randomizer(x, y, 60, 56, 0, 0);
				}
				//After finding the correct coordinates then place
			}
		}
		Barrels.push_back(make_unique<Barrel>(x, y));
		masterList.push_back(make_unique<coor>(x, y));
		Barrels[i]->setWorld(this);
	}

	
}
void StudentWorld::Randomizer(int &x, int &y, int xRng, int yRng, int xDom, int yDom) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> Xposition(xDom, xRng);
	std::uniform_int_distribution<std::mt19937::result_type> Yposition(yDom, yRng);
	x = Xposition(rng);
	y = Yposition(rng);
}
void StudentWorld::BarrelsDo() {
	for (int i = 0; i < Barrels.size(); i++)
	{
		Barrels[i]->doSomething();
	}
}
void StudentWorld::GoldDo() {
	for (int i = 0; i < Golds.size(); i++)
	{
		Golds[i]->doSomething();
	}
	for (int i = 0; i < dGolds.size(); i++) {
		dGolds[i]->doSomething();
	}
}
bool StudentWorld::BoulderIceCheck(int x, int y) {
	if (!iceMap[y - 1][x]->isVisible() && !iceMap[y - 1][x + 1]->isVisible()
		&& !iceMap[y - 1][x + 2]->isVisible() && !iceMap[y - 1][x + 3]->isVisible())
	{
		return false;
	}
	else
		return true;
}
void StudentWorld::BoulderMain() {
	//Check to see if there is ice under the boulder
	for (int i = 0; i < Boulders.size(); i++) {
		if (!Boulders[i]->Stability()) {
			//Check to see if the boulder is already falling
			if (Boulders[i]->getTimer() == 30) {
				Boulders[i]->setFalling(true);	
			//Add sound once items are deleted after each tick
			}
			
			if(Boulders[i]->isFalling()){
				//If there is no ice beneath just keep falling
				if (!BoulderIceCheck(Boulders[i]->getX(), Boulders[i]->getY())) {
					Boulders[i]->moveTo(Boulders[i]->getX(), Boulders[i]->getY() - 1);
					if (!checkForBoulders(Player->getX(), Player->getY()))
						killPlayer();

				}
				//If there is Ice or another Boulder kill it
				else {
					Boulders[i]->setAlive(false);
					//Temp to see if it works
					Boulders[i]->setVisible(false);
				}
			}
			else {
				Boulders[i]->incrementTimer();
					}
			}
		else {
			//Check to see if there is ice under the boulder
			if (!BoulderIceCheck(Boulders[i]->getX(), Boulders[i]->getY()))
			{
				Boulders[i]->setStability(false);
				
			}
		}
	}
}

void StudentWorld::placeBoulders(int lvl) {
	int L = (lvl/2) + 2; // number of boulders
	if (L > 9) {
		L = 9;
	}
	// place L boulders within ice map
	for (int i = 0; i < L; i++) {
		int x, y;
		Randomizer(x, y,60,56,0,20);
	
		for (int e = 0; e < masterList.size(); e++) {
			if (checkIfTooClose(masterList[e]->getX(), masterList[e]->getY(), x, y)
				|| 8 <= y && y <= 64 && 26 <= x && x <= 33)
			{
				while (checkIfTooClose(masterList[e]->getX(), masterList[e]->getY(), x, y)
					|| 8 <= y && y <= 64 && 26 <= x && x <= 33)
				{
					Randomizer(x, y, 60, 56, 0, 20);
				}
			}
		}
		Boulders.push_back(make_unique<Boulder>(x, y));
		//Start clearing the 4x4 square for the ice for the 

		for (int i = 0; i < 4; i++) {
			for (int e = 0; e < 4; e++) {
				iceMap[y + i][x + e]->setVisible(false);
			}
		}
		Boulders[i]->setWorld(this);
		//Put the new object in the master list then check to see if its at a radius of 6
		masterList.push_back(make_unique<coor>(x, y));
	}
}