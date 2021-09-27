#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Earth;
class TunnelMan;
class GameObject;

class StudentWorld : public GameWorld
{
public:

	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	bool earthExists(int x, int y);

	bool checkOverlap(int x, int y);
	void processOverlap();

	void collectGoodie(Goodie* g);
	void foundgold(Goodie* g);
	void dropnugget();
	bool getlevelstatus();
	void removeDeadGameObjects();
	void updateDisplayText();
	int getbarrelsleft();
	void spawnsquirt(int x, int y);
	bool annoyprotester(int x, int y);


	void annoyplayer(int x, int y);
	bool pwithindistance(int x, int y);
	bool checkifindir(int x, int y, char dir);
	bool inlinewithplayer(int x, int y);
	bool hasclearpath(int x, int y);
	std::string finddirection(int x, int y);

	bool boulderdies(int x, int y);
	void bonk(int x, int y);
	bool withinboulder(int x, int y);
	bool pwithinboulder(int x, int y);

	void sonarcharge(int x, int y);

private:
	bool levelcomplete;
	int barrelsleft;

	Earth* earths[64][64];
	TunnelMan* p;

	int m_num_objects;
	std::vector<GameObject*> m_objects;

	void spawnitems(int level);
	std::string stringformatter(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);

	int tickssincelastP;
	bool spawnnewactors();

	int mprotesters;



};

#endif // STUDENTWORLD_H_
