#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

actor::~actor()
{
	// TODO: destroy actor an dyn mem
}

void actor::doSomething()// HERE JUST FOR DEBUGGING, should be an Abstract func
{
}

bool actor::setWorld(StudentWorld *world)
{
	this->m_sWorld = world;
	return false;
}

bool actor::isAlive()
{
	return (is_alive);
}

void actor::setAlive(bool AorD)
{
	is_alive = AorD;
}

Ice::~Ice()
{
	// destroy the ice
}

void Ice::doSomething()
{
	// only thing Ice could do is mark itself for deletion
}


//Person Base Class
Person::~Person()
{
	// Todo: delete the person and any dyn mem
}
bool Person::checkBounds(int x, int y) {
	if (x > 60)
		return false;
	if (y > 60)
		return false;
	if (x < 0)
		return false;
	if (y < 0)
		return false;
}
bool Person::basicAction(int action)
{ //Add Boulder checking to make sure cant move there
	// do stuff based on the input otherwise do nothing
	int tempX, tempY;

	switch (action) {
		//Sonar Key Press
	case('z'):
		m_sWorld->SonarAction();
		return true;
	case('Z'):
		m_sWorld->SonarAction();
		return true;
		//Gold dropping
	case(KEY_PRESS_TAB):
		m_sWorld->goldDrop(); //call public world func
		return true;
		//Squirts
	case(KEY_PRESS_SPACE):
		//Call on world to create the squirt object
		m_sWorld->SquirtGunFire();
		return true;
	case(KEY_PRESS_UP):
		// move up if possible
		//Get the current coordinates
		tempX = getX();
		tempY = getY();
		//Increment based on direction
		//Check the boundaries with the function and return
		if (getDirection() != up) {
			setDirection(up);
			return true;
		}
		if (checkBounds(tempX, tempY + 1) && m_sWorld->checkForBoulders(tempX, tempY + 1)) {
			moveTo(tempX, tempY + 1);
		}
		else
			return false;
		return true;
	case(KEY_PRESS_RIGHT):
		// move right if possible
		//Get the current coordinates
		tempX = getX();
		tempY = getY();
		//Increment based on direction
		//Check the boundaries with the function and return
		if (getDirection() != right) {
			setDirection(right);
			return true;
		}
		if (checkBounds(tempX + 1, tempY) && getDirection() == right 
			&& m_sWorld->checkForBoulders(tempX + 1, tempY)) {
			moveTo(tempX + 1, tempY);
		}
		else
			return false;
		return true;
	case(KEY_PRESS_DOWN):
		// move down if possible
		//Get the current coordinates
		tempX = getX();
		tempY = getY();
		//Increment based on direction
		//Check the boundaries with the function and return
		if (getDirection() != down) {
			setDirection(down);
			return true;
		}

		if (checkBounds(tempX, tempY - 1) && m_sWorld->checkForBoulders(tempX, tempY - 1)) {
			moveTo(tempX, tempY - 1);

		}
		else
			return false;
		return true;
	case(KEY_PRESS_LEFT):
		// move left if possible
		//Get the current coordinates
		tempX = getX();
		tempY = getY();
		//Increment based on direction
		//Check the boundaries with the function and return
		if (getDirection() != left) {
			setDirection(left);
			return true;
		}
		if (checkBounds(tempX - 1, tempY) && getDirection() == left 
			&& m_sWorld->checkForBoulders(tempX - 1, tempY)) {
			moveTo(tempX - 1, tempY);
		}
		else
			return false;

		return true;
	}
	return false;
}

//Iceman Base Class
Iceman::~Iceman()
{
	// todo delete the Iceman and any dyn mem
}
int Iceman::getHealth() { return health; }
int Iceman::getSquirtsLeft() { return squirts; }
int Iceman::getPlayerGoldCount() { return gold; }
int Iceman::getPlayerSonarChargeCount() { return sonar; }
void Iceman::doSomething(int action)
{
	if (!this->isAlive()) {
		return;
	}
	// do stuff based on input
	basicAction(action);
}
int Iceman::getPlayerDir() {
	Direction dir = getDirection();
	switch (dir) {
	case(up):
		return 1;
	case(down):
		return 2;
	case(left):
		return 3;
	case(right):
		return 4;
	}
}

//Goodies Class
Goodies::~Goodies()
{
}
//Protesters Class
void Protester::doSomething(char action)
{
	if (basicAction(action)) { // action is done in basic action if it was a basic action
		return;
	}
}

void Gold::doSomething()
{
	if (timer == 0)
		setAlive(false);
	if (!isAlive())
		return;
	double radius = m_sWorld->distanceFunc(this);
//If not perm
	if (!isPerm) {
		//if (radius <= 3.00) {
			//Insert code to kill protester
			//setAlive(false);
			//m_sWorld->playSound(SOUND_PROTESTER_FOUND_GOLD);
			//m_sWorld->increaseScore(25);
		//}
		//else
			timer--;
	}


	if (!isVisible() && radius <= 4.00 && isPerm) {
		this->setVisible(true);
		return;
	}
	else if (radius <= 3.00 && isPerm) {
		this->setAlive(false);
		m_sWorld->playSound(SOUND_GOT_GOODIE);
		m_sWorld->increaseScore(10);
		m_sWorld->incrementGold();
	}

}

void Barrel::doSomething()
{
	if (!(this->isAlive()))
		return;
	double radius =  m_sWorld->distanceFunc(this); 
	if ((!this->isVisible()) && radius <= 4.00) {
		this->setVisible(true);
		return;
	}
	else if (radius <= 3.00){ 
		this->setAlive(false);
		m_sWorld->playSound(SOUND_FOUND_OIL);
		m_sWorld->increaseScore(1000);
	
	}
}

void SonarKit::doSomething()
{
	if (!isAlive())
		return;
	if (timer == 0)
		setAlive(false);
	//Get radius
	double radius = m_sWorld->distanceFunc(this);
	//If within a radius of 3.0 activate
	if (radius <= 3.00) {
		//Insert code to kill protester
		setAlive(false);
		m_sWorld->playSound(SOUND_GOT_GOODIE);
		m_sWorld->increaseScore(75);
		m_sWorld->incrementSonar();
	}
	timer--;
}

void WaterSpurt::doSomething()
{
	if (!isAlive())
		return;
	if (distance == 0)
		setAlive(false);
	//If still alive travel
	switch (dir) {
	case(up):
		if (m_sWorld->squirtTraversal(1)) {
			moveTo(getX(), getY() + 1);
			DecrementDistance();
		}
		else
			setAlive(false);
		break;
	case(down):
		if (m_sWorld->squirtTraversal(2)){
			moveTo(getX(), getY() - 1);
		DecrementDistance();
	}
		else
			setAlive(false);
		break;
	case(left):
		if (m_sWorld->squirtTraversal(3)){
			moveTo(getX() - 1, getY());
		DecrementDistance();
}
		else
			setAlive(false);
		break;
	case(right):
		if (m_sWorld->squirtTraversal(4)){
			moveTo(getX() + 1, getY());
		DecrementDistance();
		}
		else
			setAlive(false);
		break;
	}

}
int WaterSpurt::getDistance() {
	return distance;
}
void WaterSpurt::setDistance(int d) {
	distance = d;
}
void WaterSpurt::DecrementDistance() { distance--; }
void WaterPool::doSomething()
{
	if (!isAlive())
		return;
	if (timer == 0)
		setAlive(false);
	//Get radius
	double radius = m_sWorld->distanceFunc(this);
	//If within a radius of 3.0 activate
	if (radius <= 3.00) {
		
		setAlive(false);
		m_sWorld->playSound(SOUND_GOT_GOODIE);
		m_sWorld->increaseScore(100);
		m_sWorld->incrementSquirt();
	}
	timer--;

}
//Boulder Members
void Boulder::doSomething()
{
	if (!isAlive())//If dead return immediately
		return;
	
}
void Boulder::setStability(bool set) {
	isStable = set;
}
bool Boulder::Stability() { return isStable; }
void Boulder::setFalling(bool set) { falling = set; }
bool Boulder::isFalling() { return falling; }
void Boulder::incrementTimer() {timer++;}
int Boulder::getTimer() { return timer; }