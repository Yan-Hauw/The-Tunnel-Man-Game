#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class GameObject : public GraphObject
{
public:
	GameObject(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~GameObject();
	virtual void doSomething() = 0;
	StudentWorld* getWorld(); //For accessing pointer to StudentWorld
	bool getVisible();
	void changeVisible(bool vis);

	//Takes in the x and y coordinates of the gameobject which through this function 
	//we are trying to ensure that a different object is greater than a 
	//length of the integer value “distance” away from our main object.
	bool beyonddistance(int x, int y, int distance);

	bool isGoodie();
	bool isBarrel();
	bool isNugget();
	bool isSonar();
	bool isWater();
	bool isBoulder();
	bool isProtester();
	virtual bool getStatus();
	void setStatus(bool dead);//Updates the bool variable isdead. This variable tells us whether or not the object we are pointing to is still within gameplay.


	//Takes in the xand y coordinates of a desired location on the oil field which we are trying to see whether if an object is placed / moved there, our object will :
	//	1.	Be out of bounds of the oil field
	//	2.	Have any of the 16 pixels of our desired object clash with an earth object that is not yet dead
	//	3.	Is within a radius of 3 of any boulder.
	bool isblocked(int x, int y); 

	int gethitpoints(); //Returns the number of hit points of our desired game object (TunnelMan or Protester) at the current point in time when this function is called. (TunnelMan or Protester).
	void sethitpoints(int amt); //Updates the number of hit points of our desired game object (TunnelMan or Protester). 
	void dechitpoints(int amt); //Decrements the number of hit points of our desired game object (TunnelMan or Protester) 

	virtual void setwait();
	virtual void settickcounter(int amt);

	
private:
	StudentWorld* world; // Studentworld pointer
	bool changedVisible; // A bool that helps us know that we have already once changed the visibility of an object
	int mimageID;
	bool isdead;
	int hitpoints;

};

class Earth : public GameObject
{
public:
	Earth(StudentWorld* w, int x, int y);
	virtual ~Earth();
	virtual void doSomething();

private:

};

class Boulder : public GameObject
{
public:
	Boulder(StudentWorld* w, int x, int y, int stable);
	virtual ~Boulder();
	virtual void doSomething();

private:
	int state;
	int waitingticks;

};

class TunnelMan : public GameObject
{
public:
	TunnelMan(StudentWorld* w);
	virtual ~TunnelMan();
	virtual void doSomething();
	int getsquirtsleft();
	void addwater(int amt);
	int getgoldleft();
	void addgold(int amt);
	int getsonarcount();
	void addsonar(int amt);
	virtual bool getStatus();

private:
	int squirtsleft;
	int goldleft;
	int sonarcount;

};

class Squirt : public GameObject
{
public:
	Squirt(StudentWorld* w, int x, int y, Direction dir);
	virtual ~Squirt();
	virtual void doSomething();

private:
	int traveldist;


};


class Goodie : public GameObject
{
public:
	Goodie(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~Goodie();
	virtual void doSomething();


private:


};

class Barrel : public Goodie
{
public:
	Barrel(StudentWorld* w, int x, int y);
	virtual ~Barrel();

	void doSomething();

private:

};


class Nugget : public Goodie
{
public:
	Nugget(StudentWorld* w, int x, int y, bool visible, int picker, int state);
	virtual ~Nugget();

	void doSomething();

private:
	int whoitsmeantfor;
	int life;
	int lifetime;

};


class Sonar : public Goodie
{
public:
	Sonar(StudentWorld* w);
	virtual ~Sonar();

	void doSomething();

private:
	int life;

};


class Water : public Goodie
{
public:
	Water(StudentWorld* w, int x, int y);
	virtual ~Water();

	void doSomething();

private:
	int life;

};


class Protester : public GameObject
{
public:

	Protester(StudentWorld* w, int level);
	virtual ~Protester();
	virtual void doSomething();
	void move(Direction d);
	virtual void setwait();
	virtual void settickcounter(int amt);

	std::string canturn();
	virtual bool getStatus();



private:
	int numSquaresToMoveInCurrentDirection;

	int tickstowait;
	int tickcounter;
	int tickssinceturn;
	int tickssinceshout;
	int reststatewait;
};







#endif // ACTOR_H_
