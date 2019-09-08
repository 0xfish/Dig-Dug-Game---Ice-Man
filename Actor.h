#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld; // forward declare 
					// CLASS CONSTANTS
const int PLAYER_HP = 10;
const int PROTESTER_HP = 5;
const int HARD_CORE_HP = 20;


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class actor : public GraphObject { // 1 
								   // ADD CODE
public:
	actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :
		GraphObject(imageID, startX, startY, dir, size, depth) {
		is_alive = true; setVisible(true);
	};
	virtual ~actor();
	virtual void doSomething() = 0; // abstract objects don't do anything
	bool setWorld(StudentWorld*);
	bool isAlive();
	void setAlive(bool);

private:
	bool is_alive;				// determins whether or not the actor is alive 
protected:
	StudentWorld * m_sWorld; // pointer to objects world
};
//Ice Class
class Ice : public actor {
public:
	Ice(int startX, int startY) : actor(IID_ICE, startX, startY, right, 0.25, 3) {};
	~Ice();
private:
	void doSomething(); //Does nothing
};

class Person : public actor {
public:
	Person(int imageID, int startX, int startY, int hp) : actor(imageID, startX, startY, right, 1, 0) { this->setVisible(true); m_hit_points = hp; };
	~Person();
	bool basicAction(int); // if this is somthing any person can do it will be in here
	bool checkBounds(int x, int y);
	void doSomething() {}; //Does nothing
private:
	int m_hit_points;
};


class Iceman : public Person {
public:
	Iceman(int startX, int startY) : Person(IID_PLAYER, startX, startY, PLAYER_HP) {};
	~Iceman();
	void doSomething(int action);
	int getHealth();
	int getGoldCount() { return gold; }
	int getSquirtsLeft();
	int getPlayerGoldCount();
	int getPlayerSonarChargeCount();
	int getPlayerDir();
	void incrementSonar() { sonar++; }
	void incrementSquirtGun() { squirts +=5; }
	void incrementGoldCount() { gold++; }
	void decrementSquirtGun() { squirts--; }
	void decrementGold() { gold--; }
	void decrementSonar() { sonar--; }
private:
	int health = PLAYER_HP;
	int squirts = 5;
	int gold = 0;
	int sonar= 1;
};

class Protester : public Person {
	// ADD CODE
	// virtual functions for some stuff
	// image id : IID_PROTESTER
public:
	Protester(int startX, int startY) :Person(IID_PROTESTER, startX, startY, PROTESTER_HP) {
		leave_oil_field = false; setDirection(left);
		HP = 5;
	};
	void doSomething(char);

private:
	bool leave_oil_field;
	int HP;
};

//class HardcoreProtester : public Protester {
//	// ADD CODE
//	// image id :IID_HARD_CORE_PROTESTER	
//public:
//		HardcoreProtester(int startX, int startY) : Protester(IID_HARD_CORE_PROTESTER, startX, startY) {};
//};

class Goodies : public actor {	// abstract
public:
	Goodies(int imageID, int startX, int startY) 
		: actor(imageID, startX, startY, right, 1.0, 2) {}; // all goodies have the same dir, size depth
	virtual ~Goodies();
private:

};

class WaterPool : public Goodies {
public:
	WaterPool(int x, int y, int level) : Goodies(IID_WATER_POOL, x, y) {
		int max = 0;
		max = 300 - (10 * level);
		if (max < 100)
			max = 100;
			timer = max;
	};
	void doSomething();
private:
	int timer;
};

class SonarKit : public Goodies {
public:
	SonarKit(int x, int y, int level) : Goodies(IID_SONAR, x, y) {

		int max = 0;
		max = (300 - (10 * level));
		if (max < 100)
			max = 100;
		timer = max;
	}
	void doSomething();
private:
	int timer;
};
// DOING RIGHT NOW
class Barrel : public Goodies {
	// ADD CODE 
	// image id : IID_BARREL 
public:
	Barrel(int startX, int startY) :Goodies(IID_BARREL, startX, startY)
	{ setVisible(true); }; // for testing 
	void doSomething();
private:
};
//Gold class
class Gold : public Goodies {
	
public:
	Gold(int startX, int startY) :Goodies(IID_GOLD, startX, startY)
	{
		timer = 100;
		isPerm = true;
		setVisible(true);
	}; // for testing 
	void setPerm(bool p) { isPerm = p; }
	void doSomething();
private:
	bool isPerm;
	int timer;
};

class Projectile : public actor {
public:
	Projectile(int imageID, int startX, int startY) :
		actor(imageID, startX, startY, down, 1.0 ,1) { this->setVisible(true); }; // direction?

private:

};

class WaterSpurt : public Projectile {
	// ADD CODE
	// image id : IID_WATER_SPURT 
public:
	WaterSpurt(int x, int y, int d) : Projectile(IID_WATER_SPURT,x,y) {
		
		switch (d) {
		case(1):
			dir = up;
			setDirection(dir);
			break;
		case(2):
			dir = down;
			setDirection(dir);
			break;
		case(3):
			dir = left;
			setDirection(dir);
			break;
		case(4):
			dir = right;
			setDirection(dir);
			break;
		}
		setVisible(true);
		distance = 4;
	};
	void setDistance(int d);
	int getDistance();
	void DecrementDistance();
	void doSomething();
private:
	Direction dir;
	int distance;
};
//DO NEXT!!
class Boulder : public Projectile { 
public:
	Boulder(int startX, int startY) : Projectile(IID_BOULDER, startX, startY)
	{};
	void doSomething();
	void setStability(bool set);
	bool Stability();
	bool isFalling();
	void setFalling(bool set);
	void incrementTimer();
	int getTimer();
private:
	bool isStable = true;
	bool falling = false;
	int timer = 0;
};


#endif // ACTOR_H_