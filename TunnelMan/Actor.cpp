#include "GraphObject.h"
#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <cmath>
#include <algorithm> 

using namespace std;

GameObject::GameObject(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) // Constructing a game object
	:GraphObject(imageID, startX, startY, dir, size, depth) // Initialise base graph object
{
	world = w;
	changedVisible = false;
	mimageID = imageID;
	isdead = false;
}

GameObject::~GameObject() // Destruct GameObject
{}

void GameObject::doSomething()
{
}

StudentWorld* GameObject::getWorld()
{
	return world;
}

bool GameObject::getVisible()
{
	return changedVisible;
}

void GameObject::changeVisible(bool vis)
{
	changedVisible = vis;
}

bool GameObject::beyonddistance(int x, int y, int distance)
{
	// Distance between x coordinates
	int xdist = abs(getX() - x);

	// Distance between y coordinates
	int ydist = abs(getY() - y);

	double totaldist = sqrt(pow(xdist, 2) + pow(ydist, 2));

	//Check if our calculated distance is smaller than what we want it to be smaller than
	if (totaldist <= distance)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool GameObject::isGoodie()
{
	if (mimageID == TID_BARREL || mimageID == TID_GOLD)
	{
		return true;
	}
	return false;
}

bool GameObject::isBarrel()
{
	if (mimageID == TID_BARREL)
	{
		return true;
	}
	return false;
}

bool GameObject::isNugget()
{
	if (mimageID == TID_GOLD)
	{
		return true;
	}
	return false;
}

bool GameObject::isSonar()
{
	if (mimageID == TID_SONAR)
	{
		return true;
	}
	return false;
}

bool GameObject::isWater()
{
	if (mimageID == TID_WATER_POOL)
	{
		return true;
	}
	return false;
}

bool GameObject::isBoulder()
{
	if (mimageID == TID_BOULDER)
	{
		return true;
	}
	return false;
}

bool GameObject::isProtester()
{
	if (mimageID == TID_PROTESTER)
	{
		return true;
	}
	return false;
}

bool GameObject::getStatus()
{
	return isdead;
}

//Updates the bool variable isdead.This variable tells us whether or not the object we are pointing to is still within gameplay
void GameObject::setStatus(bool dead)
{
	isdead = dead;
}

//Takes in the xand y coordinates of a desired location on the oil field which we are trying to see whether if an object is placed / moved there, our object will :
bool GameObject::isblocked(int x, int y)
{
	//	1.	Be out of bounds of the oil field
	if (x > 60 || x < 0 || y > 60 || y < 0)
	{
		return true;
	}
	//	2.	Have any of the 16 pixels of our desired object clash with an earth object that is not yet dead
	if (getWorld()->checkOverlap(x, y) == true)
	{
		return true;
	}
	//	3.	Is within a radius of 3 of any boulder.
	if (getWorld()->withinboulder(x, y) == true)
	{
		return true;
	}
	return false;
}

int GameObject::gethitpoints()
{
	return hitpoints;
}

void GameObject::sethitpoints(int amt)
{
	hitpoints = amt;
}

void GameObject::dechitpoints(int amt)
{
	hitpoints = hitpoints - amt;
}

void GameObject::setwait()
{
}

void GameObject::settickcounter(int amt)
{
}




Earth::Earth(StudentWorld* w, int x, int y) // Constructing an earth
	: GameObject(w, TID_EARTH, x, y, right, .25, 3) 
{
	setVisible(true);
}

Earth::~Earth() // Earth destructor
{}

void Earth::doSomething()
{
	return;
}


Boulder::Boulder(StudentWorld* w, int x, int y, int stable) 
	: GameObject(w, TID_BOULDER, x, y, down, 1.0, 1) // Initialising base game object
{
	setVisible(true);
	state = stable;
	waitingticks = 30;
}

Boulder::~Boulder() 
{}


void Boulder::doSomething()
{
	if (getStatus() == true)
	{
		return;
	}
	// If in stable state
	if (state == 0)
	{
		//check if the Earths underneath it are still in play.
		bool supported = false;

		for (int i = getX(); i < getX() + 4; i++)
		{
			if (getWorld()->earthExists(i, getY() - 1) == true)
			{
				supported = true;
			}
		}
		if (supported == false)
		{
			state = 1;
		}
	}
	// If in waiting state
	if (state == 1)
	{
		if (waitingticks > 0)
		{
			waitingticks--;
		}
		else//if waiting time has ended,
		{
			//change state to falling stateand play sound.
			state = 2;
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
	}
	if (state == 2)
	{
		//moves the boulder as necessary
		moveTo(getX(), getY() - 1);

		if (getWorld()->boulderdies(getX(), getY()) == true)//If the boulder has to disappear,
		{
			setStatus(true);//disappear.
		}

		//if the boulder can bonk a player / protestor,
		//bonk.
		getWorld()->bonk(getX(), getY());

	}
}


TunnelMan::TunnelMan(StudentWorld* w)
	: GameObject(w, TID_PLAYER, 30, 60, right, 1.0, 0)
{
	setVisible(true);
	squirtsleft = 5;
	goldleft = 0;
	sonarcount = 1;
	sethitpoints(10);
}

TunnelMan::~TunnelMan()
{
}

void TunnelMan::doSomething()
{
	// If not alive, 
	// return
	if (getStatus() == true)
	{
		return;
	}


	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
			//For every direction we try to move in,
			//	check if the movement is within bounds,
			//	then check if the movement will make us move into the boulder.
			//	If yes to both above, allow movement.
		case KEY_PRESS_LEFT:
			if (getDirection() != left) // if not yet facing left, make it face left
			{
				setDirection(left);
				break;
			}
			if (getX() >= 1)
			{
				if (getWorld()->pwithinboulder(getX() - 1, getY()) == false)
				{
					moveTo(getX() - 1, getY());
				}
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right) // if not yet facing right, make it face right
			{
				setDirection(right);
				break;
			}
			if (getX() <= 59)
			{
				if (getWorld()->pwithinboulder(getX() + 1, getY()) == false)
				{
					moveTo(getX() + 1, getY());
				}
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up) // if not yet facing up, make it face up
			{
				setDirection(up);
				break;
			}
			if (getY() <= 59)
			{
				if (getWorld()->pwithinboulder(getX(), getY() + 1) == false)
				{
					moveTo(getX(), getY() + 1);
				}
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down) // if not yet facing down, make it face down
			{
				setDirection(down);
				break;
			}
			if (getY() >= 1)
			{
				if (getWorld()->pwithinboulder(getX(), getY() - 1) == false)
				{
					moveTo(getX(), getY() - 1);
				}
			}
			break;
		
		case KEY_PRESS_SPACE:
			if (squirtsleft > 0)//If we have water left,
			{
				if (getDirection() == left)
				{
					getWorld()->spawnsquirt(getX() - 4, getY());
				}
				if (getDirection() == right)
				{
					getWorld()->spawnsquirt(getX() + 4, getY());
				}
				if (getDirection() == up)
				{
					getWorld()->spawnsquirt(getX(), getY() + 4);
				}
				if (getDirection() == down)
				{
					getWorld()->spawnsquirt(getX(), getY() - 4);
				}
				//	if we fire a squirt,
				//	create a new squirt travelling in the correct direction
				squirtsleft--; // we now have one less squirt
			}
			break;
		case KEY_PRESS_TAB:
			if (goldleft > 0)
			{
				getWorld()->dropnugget();
				goldleft--;
			}
			break;
		case 'Z':
		case'z':
			if (sonarcount > 0)
			{
				getWorld()->sonarcharge(getX(), getY());
				sonarcount--;

			}
			break;


		}
	}
}

int TunnelMan::getsquirtsleft()
{
	return squirtsleft;
}

int TunnelMan::getgoldleft()
{
	return goldleft;
}

int TunnelMan::getsonarcount()
{
	return sonarcount;
}

bool TunnelMan::getStatus()
{
	if (gethitpoints() <= 0)
	{
		return true;
	}
	return false;
}

void TunnelMan::addgold(int amt)
{
	goldleft = goldleft + amt;
}

void TunnelMan::addsonar(int amt)
{
	sonarcount = sonarcount + amt;
}

void TunnelMan::addwater(int amt)
{
	squirtsleft = squirtsleft + amt;
}



Squirt::Squirt(StudentWorld* w, int x, int y, Direction dir)
	: GameObject(w, TID_WATER_SPURT, x, y, dir, 1.0, 1) // Initialise an actor according to details in spec
{
	setVisible(true);
	traveldist = 4;

}

Squirt::~Squirt()
{
}

void Squirt::doSomething()
{
	if (getWorld()->annoyprotester(getX(), getY()) == true)
	{
		setStatus(true);
		return;
	}
	if (traveldist == 0)
	{
		setStatus(true);
		return;
	}
	if (getDirection() == left)
	{
		if (isblocked(getX() - 1, getY()))
		{
			setStatus(true);
			return;
		}
		else
		{
			moveTo(getX() - 1, getY());
			traveldist--;
			return;
		}
	}
	else if (getDirection() == right)
	{
		if (isblocked(getX() + 1, getY()))
		{
			setStatus(true);
			return;
		}
		else
		{
			moveTo(getX() + 1, getY());
			traveldist--;
			return;
		}
	}
	else if (getDirection() == up)
	{
		if (isblocked(getX(), getY() + 1))
		{
			setStatus(true);
			return;
		}
		else
		{
			moveTo(getX(), getY() + 1);
			traveldist--;
			return;
		}
	}
	else if (getDirection() == down)
	{
		if (isblocked(getX(), getY() - 1))
		{
			setStatus(true);
			return;
		}
		else
		{
			moveTo(getX(), getY() - 1);
			traveldist--;
			return;
		}
	}
}


Goodie::Goodie(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	: GameObject(w, imageID, startX, startY, dir, size, depth) // Initialise an actor according to details in spec
{
}

Goodie::~Goodie()
{
}

void Goodie::doSomething()
{
}


Barrel::Barrel(StudentWorld* w, int x, int y)
	:Goodie(w, TID_BARREL, x, y, right, 1.0, 2)
{
	setVisible(false);
}

Barrel::~Barrel()
{
}

void Barrel::doSomething()
{
	getWorld()->collectGoodie(this);
}


Nugget::Nugget(StudentWorld* w, int x, int y, bool visible, int picker, int state)
	:Goodie(w, TID_GOLD, x, y, right, 1.0, 2)
{
	setVisible(visible);
	whoitsmeantfor = picker;
	life = state;
	lifetime = 100;
}

Nugget::~Nugget()
{
}

void Nugget::doSomething()
{
	// 0 represents the TunnelMan
	if (whoitsmeantfor == 0)
	{
		getWorld()->collectGoodie(this);
		return;
	}
	if (whoitsmeantfor == 1)
	{
		if (lifetime <= 0)
		{
			setStatus(true);
			return;
		}
		getWorld()->foundgold(this);
		lifetime--;
	}

}


Sonar::Sonar(StudentWorld* w) // Constructing an earth
	: Goodie(w, TID_SONAR, 0, 60, right, 1.0, 2) // Initialising base game object
{
	setVisible(true);
	int level = (int)(getWorld()->getLevel());
	life = max(100, 300 - 10 * level);
}

Sonar::~Sonar() // Earth destructor
{}

void Sonar::doSomething()
{
	if (life <= 0)
	{
		setStatus(true);
		return;
	}
	getWorld()->collectGoodie(this);
	life--;
	return;
}


Water::Water(StudentWorld* w, int x, int y) // Constructing an earth
	: Goodie(w, TID_WATER_POOL, x, y, right, 1.0, 2) // Initialising base game object
{
	setVisible(true);
	int level = (int)(getWorld()->getLevel());
	life = max(100, 300 - 10 * level);
}

Water::~Water() // Earth destructor
{}

void Water::doSomething()
{
	if (life <= 0)
	{
		setStatus(true);
		return;
	}
	getWorld()->collectGoodie(this);
	life--;
	return;
}


Protester::Protester(StudentWorld* w, int level)
	: GameObject(w, TID_PROTESTER, 60, 60, left, 1.0, 0) // Initialise an actor according to details in spec
{
	setVisible(true);
	sethitpoints(5);
	setDirection(left);
	numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
	tickstowait = max(0, 3 - level / 4);
	tickcounter = 0;
	tickssinceturn = 200;
	tickssinceshout = 15;
	reststatewait = 0;
}

Protester::~Protester()
{
}

void Protester::move(Direction d)
{
	if (d == right)
	{
		setDirection(right);
		if (getX() <= 59)
		{
			moveTo(getX() + 1, getY());
		}
	}
	if (d == left)
	{
		setDirection(left);
		if (getX() >= 1)
		{
			moveTo(getX() - 1, getY());
		}
	}
	if (d == up)
	{
		setDirection(up);
		if (getY() <= 59)
		{
			moveTo(getX(), getY() + 1);
		}
	}
	if (d == down)
	{
		setDirection(down);
		if (getY() >= 1)
		{
			moveTo(getX(), getY() - 1);
		}
	}
}


string Protester::canturn()
{
	bool done0 = false;
	bool done1 = false;

	switch (getDirection())
	{
	case right:
		do
		{
			int random = rand() % 2;
			if (random == 0)
			{
				done0 = true;
				if (!isblocked(getX(), getY() + 1))
				{
					return "up";
				}
			}
			else if (random == 1)
			{
				done1 = true;
				if (!isblocked(getX(), getY() - 1))
				{
					return "down";
				}
			}
		} while (done0 == false || done1 == false);
		break;
	case left:
		do
		{
			int random = rand() % 2;
			if (random == 0)
			{
				done0 = true;
				if (!isblocked(getX(), getY() + 1))
				{
					return "up";
				}
			}
			else if (random == 1)
			{
				done1 = true;
				if (!isblocked(getX(), getY() - 1))
				{
					return "down";
				}
			}
		} while (done0 == false || done1 == false);
		break;
	case up:
		do
		{
			int random = rand() % 2;
			if (random == 0)
			{
				done0 = true;
				if (!isblocked(getX() - 1, getY()))
				{
					return "left";
				}
			}
			else if (random == 1)
			{
				done1 = true;
				if (!isblocked(getX() + 1, getY()))
				{
					return "right";
				}
			}
		} while (done0 == false || done1 == false);
		break;
	case down:

		do
		{
			int random = rand() % 2;
			if (random == 0)
			{
				done0 = true;
				if (!isblocked(getX() - 1, getY()))
				{
					return "left";
				}
			}
			else if (random == 1)
			{
				done1 = true;
				if (!isblocked(getX() + 1, getY()))
				{
					return "right";
				}
			}
		} while (done0 == false || done1 == false);
		break;
	}

	return "";

}

void Protester::doSomething()
{
	// if protester is dead, return
	if (getStatus() == true)
	{
		return;
	}

	// if protester is in rest state,
	if (reststatewait > 0)
	{
		reststatewait--;
		return;
	}
	else if (tickcounter % tickstowait != 0)
	{
		tickcounter++;
		return;
	}

	tickcounter++;

	// Check if can shout at TunnelMan
	// If yes, shout
	if (tickssinceshout >= 15 && getWorld()->pwithindistance(getX(), getY()) == true)
	{
		// still a non-resting tick so increment ticks since previous perpendicular turn
		tickssinceturn++;
		if (getDirection() == left)
		{
			if (getWorld()->checkifindir(getX(), getY(), 'l') == true)
			{
				getWorld()->annoyplayer(getX(), getY());
				tickssinceshout = 0;
				return;
			}
			else
			{
			}
		}
		else if (getDirection() == right)
		{
			if (getWorld()->checkifindir(getX(), getY(), 'r') == true)
			{
				getWorld()->annoyplayer(getX(), getY());
				tickssinceshout = 0;
				return;
			}
			else
			{
			}
		}
		else if (getDirection() == up)
		{
			if (getWorld()->checkifindir(getX(), getY(), 'u') == true)
			{
				getWorld()->annoyplayer(getX(), getY());
				tickssinceshout = 0;
				return;
			}
			else
			{
			}
		}
		else if (getDirection() == down)
		{
			if (getWorld()->checkifindir(getX(), getY(), 'd') == true)
			{
				getWorld()->annoyplayer(getX(), getY());
				tickssinceshout = 0;
				return;
			}
			else
			{
			}
		}

	}


	// If protester is in straight line of sight to player
	if (getWorld()->inlinewithplayer(getX(), getY()) == true)
	{
		if (getWorld()->pwithindistance(getX(), getY()) == false)
		{
			if (getWorld()->hasclearpath(getX(), getY()) == true)
			{
				// still a non-resting tick so increment ticks since previous perpendicular turn and shout
				tickssinceturn++;
				tickssinceshout++;
				if (getWorld()->finddirection(getX(), getY()) == "left")
				{
					setDirection(left);
					moveTo(getX() - 1, getY());
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
				if (getWorld()->finddirection(getX(), getY()) == "right")
				{
					setDirection(right);
					moveTo(getX() + 1, getY());
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
				if (getWorld()->finddirection(getX(), getY()) == "up")
				{
					setDirection(up);
					moveTo(getX(), getY() + 1);
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
				if (getWorld()->finddirection(getX(), getY()) == "down")
				{
					setDirection(down);
					moveTo(getX(), getY() - 1);
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
			}
		}
	}




	if (tickssinceturn > 200 && canturn() != "")
	{
		string turndir = canturn();
		if (turndir == "right")
		{
			setDirection(right);
		}
		if (turndir == "left")
		{
			setDirection(left);
		}
		if (turndir == "up")
		{
			setDirection(up);
		}
		if (turndir == "down")
		{
			setDirection(down);
		}
		numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
		tickssinceturn = 0;
		move(getDirection());
		numSquaresToMoveInCurrentDirection--;
		// still a non-resting tick so increment ticks since previous perpendicular turn and shout
		tickssinceshout++;

	}
	else if (numSquaresToMoveInCurrentDirection != 0)
	{
		int desx = 0;
		int desy = 0;

		switch (getDirection())
		{
		case right:
			desx = getX() + 1;
			desy = getY();
			break;
		case left:
			desx = getX() - 1;
			desy = getY();
			break;
		case up:
			desx = getX();
			desy = getY() + 1;
			break;
		case down:
			desx = getX();
			desy = getY() - 1;
			break;
		}
		if (isblocked(desx, desy) == true)
		{
			numSquaresToMoveInCurrentDirection = 0;
			// Then does nothing during the current tick
		}
		else
		{
			move(getDirection());
			numSquaresToMoveInCurrentDirection--;
		}
		// still a non-resting tick so increment ticks since previous perpendicular turn and shout
		tickssinceturn++;
		tickssinceshout++;
	}
	else if (numSquaresToMoveInCurrentDirection == 0)
	{
		int desx = 0;
		int desy = 0;
		do {
			int random = rand() % 4;
			switch (random)
			{
			case 0:
				setDirection(right);
				desx = getX() + 1;
				desy = getY();
				break;
			case 1:
				setDirection(left);
				desx = getX() - 1;
				desy = getY();
				break;
			case 2:
				setDirection(up);
				desx = getX();
				desy = getY() + 1;
				break;
			case 3:
				setDirection(down);
				desx = getX();
				desy = getY() - 1;
				break;
			}
		} while (isblocked(desx, desy) == true);

		numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
		// still a non-resting tick so increment ticks since previous perpendicular turn
		tickssinceturn++;
		tickssinceshout++;

		move(getDirection());
		numSquaresToMoveInCurrentDirection--;
	}
}


void Protester::setwait()
{
	int maximum = (int)(100 - getWorld()->getLevel() * 10);
	reststatewait = max(50, maximum);
}

bool Protester::getStatus()
{
	if (gethitpoints() <= 0)
	{
		return true;
	}
	return false;
}

void Protester::settickcounter(int amt)
{
	tickcounter = amt;
}

