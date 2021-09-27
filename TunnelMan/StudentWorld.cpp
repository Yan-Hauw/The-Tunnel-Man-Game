#include "StudentWorld.h"
#include "Actor.h"
#include "GraphObject.h"
#include <string>
#include <algorithm> 
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

void StudentWorld::spawnitems(int level) //Spawn items upon studentWorld creation
{
	int barrels = min(2 + level, 21);	//Decide how many barrels to spawn, given the current level
	barrelsleft = barrels;

	for (int i = 0; i < barrels; i++)
	{
		int x;	//Randomly generate a spawn location
		int y;

		bool tooclose = false;
		bool withinbounds = true;

		do
		{
			// Generate valid random spawn location for a barrel
			x = rand() % 61;
			y = rand() % 57;

			tooclose = false;
			withinbounds = true;

			for (int j = 0; j < m_num_objects; j++)
			{
				if (m_objects[j] != nullptr)
				{
					if (m_objects[j]->isGoodie() == true) // Check if the above generated spawn location is too close to another goodie
					{
						if (m_objects[j]->beyonddistance(x, y, 6) == false)
						{
							tooclose = true;
						}
					}
				}
			}

			// Barrels cannot exist in the centre shaft
			if (x == 27 || x == 28 || x == 29 || x == 30 || x == 31 || x == 32 || x == 33) 
			{
				withinbounds = false;
			}

		} while (tooclose == true || withinbounds == false);

		// Make a new barrel and insert it into the objects vector
		Barrel* b = new Barrel(this, x, y);
		m_objects.push_back(b);
		m_num_objects++;
	}

	int nuggets = max(5 - level/2, 2);	//Decide how many nuggets to spawn, given the current level

	for (int i = 0; i < nuggets; i++)
	{
		int x;	//Randomly generate a spawn location
		int y;

		bool tooclose = false;
		bool withinbounds = true;

		do
		{
			// Generate valid random spawn location for a nugget
			x = rand() % 61;
			y = rand() % 57;

			tooclose = false;
			withinbounds = true;

			for (int j = 0; j < m_num_objects; j++)
			{
				if (m_objects[j] != nullptr)
				{
					if (m_objects[j]->isGoodie() == true) // Check if the above generated spawn location is too close to another goodie
					{
						if (m_objects[j]->beyonddistance(x, y, 6) == false)
						{
							tooclose = true;
						}
					}
				}
			}

			//  cannot exist in the centre shaft
			if (x == 27 || x == 28 || x == 29 || x == 30 || x == 31 || x == 32 || x == 33)
			{
				withinbounds = false;
			}

		} while (tooclose == true || withinbounds == false);


		Nugget* n = new Nugget(this, x, y, false, 0, 0);
		m_objects.push_back(n);
		m_num_objects++;
	}

	int boulders = min(level / 2 + 2, 9);	//Decide how many boulders to spawn, given the current level

	for (int i = 0; i < boulders; i++)
	{
		int x;	//Randomly generate a spawn location
		int y;

		bool tooclose = false;
		bool withinbounds = true;

		do
		{
			// Generate valid random spawn location 
			x = rand() % 61;
			y = rand() % 37 + 20;

			tooclose = false;
			withinbounds = true;

			for (int j = 0; j < m_num_objects; j++)
			{
				if (m_objects[j] != nullptr)
				{
					if (m_objects[j]->isGoodie() == true || m_objects[j]->isBoulder() == true) // Check if the above generated spawn location is too close to another distributed object
					{
						if (m_objects[j]->beyonddistance(x, y, 6) == false)
						{
							tooclose = true;
						}
					}
				}
			}

			// annot exist in the centre shaft
			if (x == 26 || x == 27 || x == 28 || x == 29 || x == 30 || x == 31 || x == 32 || x == 33 || x == 34)
			{
				withinbounds = false;
			}

		} while (tooclose == true || withinbounds == false);

		for (int i = x; i < x + 4; i++) // For the 16 squares of Earth at bottom of well/shaft
		{
			for (int j = y; j < y + 4; j++)
			{
				earths[j][i]->setVisible(false);
				earths[j][i]->changeVisible(true); // see code in GameObject base class
			}
		}


		Boulder* Bo = new Boulder(this, x, y, 0);
		m_objects.push_back(Bo);
		m_num_objects++;
	}

}


bool StudentWorld::spawnnewactors()
{
	int level = getLevel();

	int chancenewgoodie = level * 25 + 300;

	if (rand() % chancenewgoodie == 1)
	{
		if (rand() % 5 == 0)
		{
			Sonar* sn = new Sonar(this);
			m_objects.push_back(sn);
			m_num_objects++;
		}
		else
		{
			int x;	//Randomly generate a spawn location
			int y;

			bool bottomleftempty = false;
			bool entirelyempty = false;
			bool boulderclash = true;

			do
			{
				// Generate valid random spawn location for water pool
				x = rand() % 61;
				y = rand() % 61;

				bottomleftempty = true;
				entirelyempty = true;
				boulderclash = true;

				if (earthExists(x, y) == true)
				{
					bottomleftempty = false;
				}

				for (int i = y; i < y + 4; i++)
				{
					for (int j = x; j < x + 4; j++)
					{
						if (earthExists(j, i) == true)
						{
							entirelyempty = false;
						}
					}
				}

				if (withinboulder(x, y) == false)
				{
					boulderclash = false;
				}

			} while (bottomleftempty == false || entirelyempty == false || boulderclash == true);

			Water* w = new Water(this, x, y);
			m_objects.push_back(w);
			m_num_objects++;
		}
	}

	// based on specification for target number of protestors
	double minim = 2 + level * 1.5;
	int minimum = (int)(minim);

	if (tickssincelastP >= max(25, 200 - level) && mprotesters < min(15, minimum))
	{
		Protester* pr = new Protester(this, level);
		m_objects.push_back(pr);
		m_num_objects++;

		mprotesters++;
		tickssincelastP = 0;

		return true;
	}

	return false;
}



int StudentWorld::init()
{
	levelcomplete = false;
	int level = getLevel();
	tickssincelastP = 0;
	mprotesters = 1;

	p = new TunnelMan(this); // Make a new TunnelMan

	for (int i = 0; i < 64; i++) // Create Earths for all squares excluding top 4 rows and centre shaft
	{
		if (i != 60 && i != 61 && i != 62 && i != 63)
		{
			for (int j = 0; j < 64; j++)
			{
				if (j != 30 && j != 31 && j != 32 && j != 33)
				{
					earths[i][j] = new Earth(this, j, i); // dynamically allocate memory to construct earth objects
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) // For the 16 squares of Earth at bottom of well/shaft
	{
		for (int j = 30; j < 34; j++)
		{
			earths[i][j] = new Earth(this, j, i); // dynamically allocate memory to construct earth objects
		}
	}

	spawnitems(level);

	Protester* pr = new Protester(this, level);
	m_objects.push_back(pr);
	m_num_objects++;



	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	// Update the Game Status Line
	updateDisplayText(); // update the score/lives/level text at screen top


	// Give each Actor a chance to do something

	p->doSomething(); // Let the player move
	processOverlap(); // Update if any Earth has been dug through

	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector and get it to do something
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->getStatus() == false)
			{
				m_objects[i]->doSomething();
			}
		}
	}

	// Remove newly-dead actors after each tick 
	removeDeadGameObjects();


	// Add new actors
	if (spawnnewactors() == true)
	{

	}
	else
	{
		//increment number of ticks passed since last Protester added
		tickssincelastP++;
	}

	if (p->getStatus() == true)
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}


	// If the player has collected all of the Barrels on the level, then
	// return the result that the player finished the level


	if (getlevelstatus() == true)
	{
		return GWSTATUS_FINISHED_LEVEL;
	}

	// the player hasn’t completed the current level and hasn’t died
	// let them continue playing the current level
	   
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete p; // remove the TunnelMan pointer

	// Remove all Earth pointers
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (earths[i][j] != nullptr) // Only remove if a pointer was created
			{
				delete earths[i][j];
				earths[i][j] = nullptr;
			}
		}
	}

	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector and delete each object
	{
		if (m_objects[i] != nullptr)
		{
			delete m_objects[i];
			m_objects[i] = nullptr;
		}
	}

	vector<GameObject*>::iterator it;
	vector<vector<GameObject*>::iterator> positions;
	for (it = m_objects.begin(); it != m_objects.end(); it++)
	{
		positions.push_back(it);
	}
	for (vector<vector<GameObject*>::iterator>::reverse_iterator itr = positions.rbegin(); itr != positions.rend(); itr++)
	{
		m_objects.erase(*itr);
	}
	m_num_objects = 0;

}

bool StudentWorld::earthExists(int x, int y)
{
	if (earths[y][x] != nullptr) // Don't check if it is a location where an Earth will never be initialised
	{
		if (earths[y][x]->getVisible() == false) // if getVisible is false means earth has never been dug through befor
		{
			return true;
		}
	}
	return false;
}

bool StudentWorld::checkOverlap(int x, int y) // Function to check if earth at that spot has previously been dug
{
	// Check for all 16 squares that the TunnelMan occupies
	for (int i = y; i < y + 4; i++)
	{
		for (int j = x; j < x + 4; j++)
		{
			if (earthExists(j, i) == true)
			{
				return true;
			}
		}
	}
	return false;
}


void StudentWorld::processOverlap()
{
	// Process for all 16 squares that the TunnelMan occupies
	if (checkOverlap(p->getX(), p->getY()) == true)
	{
		for (int i = p->getY(); i < p->getY() + 4; i++)
		{
			for (int j = p->getX(); j < p->getX() + 4; j++)
			{
				if (earths[i][j] != nullptr) // Don't process if it is a location where an Earth will never be initialised
				{
					playSound(SOUND_DIG);
					earths[i][j]->setVisible(false);
					earths[i][j]->changeVisible(true); // see code in GameObject base class
				}
			}
		}
	}
}

void StudentWorld::collectGoodie(Goodie* g)
{
	if (g->getStatus() == true) // Check if the goodie has already been collected
	{
		return;
	}

	//if the goodie has not been collected AND the player is close enough to discover the goodie
	// Only for nuggets and barrels
	if (g->isSonar() == false && g->isWater() == false)
	{
		if (g->getVisible() == false && g->beyonddistance(p->getX(), p->getY(), 4) == false)
		{
			g->setVisible(true);
			g->changeVisible(true);
			return;
		}
	}

	// If the player reaches a position where it can pick up the goodie, pick it up
	if (g->beyonddistance(p->getX(), p->getY(), 3) == false)
	{
		g->setStatus(true); // set goodie's status to isDead
		if (g->isBarrel() == true)
		{
			playSound(SOUND_FOUND_OIL);
			// increase score by 1000
			increaseScore(1000);
			barrelsleft--;
		}
		if (g->isNugget() == true)
		{
			playSound(SOUND_GOT_GOODIE);

			increaseScore(10);
			p->addgold(1);
		}
		if (g->isSonar() == true)
		{
			playSound(SOUND_GOT_GOODIE);

			increaseScore(75);
			p->addsonar(2);
		}
		if (g->isWater() == true)
		{
			playSound(SOUND_GOT_GOODIE);

			increaseScore(100);
			p->addwater(5);
		}

		g->setVisible(false);
	}


}

void StudentWorld::foundgold(Goodie* g)
{
	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isProtester() == true) // Check if it is a protestor
			{
				if (g->beyonddistance(m_objects[i]->getX(), m_objects[i]->getY(), 3) == false)
				{
					g->setStatus(true); 
					playSound(SOUND_PROTESTER_FOUND_GOLD);


					increaseScore(25);
					m_objects[i]->dechitpoints(10);

					return;
				}
			}
		}
	}

}

void StudentWorld::dropnugget()
{
	Nugget* n = new Nugget(this, p->getX(), p->getY(), true, 1, 1);
	m_objects.push_back(n);
	m_num_objects++;



}


bool StudentWorld::getlevelstatus()
{
	bool completed = true;

	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isBarrel() == true) // Check if it is a barrel
			{
				if (m_objects[i]->getStatus() == false) // If the barrel is not dead
				{
					completed = false;
				}
			}
		}
	}

	return completed;
}

void StudentWorld::removeDeadGameObjects() // As stated in spec, remove newly dead actors after each tick
{
	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->getStatus() == true)
			{
				delete m_objects[i];
				m_objects[i] = nullptr;
			}

		}
	}
}

void StudentWorld::updateDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = p->gethitpoints() * 10;
	int squirts = p->getsquirtsleft();
	int gold = p->getgoldleft();
	int barrelsLeft = getbarrelsleft();
	int sonar = p->getsonarcount();
	int score = getScore();



	string s = stringformatter(level, lives, health, squirts, gold, barrelsLeft, sonar, score);

	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

int StudentWorld::getbarrelsleft()
{
	return barrelsleft;
}

string StudentWorld::stringformatter(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
{
	string s;
	string scoreprefix;
	if (score / 10 == 0)
	{
		s = to_string(score);
		scoreprefix = "00000";
	}
	else if (score / 100 == 0)
	{
		s = to_string(score);
		scoreprefix = "0000";
	}
	else if (score / 1000 == 0)
	{
		s = to_string(score);
		scoreprefix = "000";
	}
	else if (score / 10000 == 0)
	{
		s = to_string(score);
		scoreprefix = "00";
	}
	else if (score / 100000 == 0)
	{
		s = to_string(score);
		scoreprefix = "0";
	}
	else
	{
		s = to_string(score);
		scoreprefix = "";
	}

	string levs;
	string levelprefix;
	if (level / 10 == 0)
	{
		levs = to_string(level);
		levelprefix = " ";
	}
	else
	{
		levs = to_string(level);
		levelprefix = "";
	}

	string life = to_string(lives);

	string h;
	string hprefix;
	if (health == 100)
	{
		h = to_string(health);
		hprefix = "";
	}
	else
	{
		h = to_string(health);
		hprefix = " ";
	}

	string w;
	string wprefix;
	if (squirts/10 == 0)
	{
		w = to_string(squirts);
		wprefix = " ";
	}
	else
	{
		w = to_string(squirts);
		wprefix = "";
	}

	string g;
	string gprefix;
	if (gold / 10 == 0)
	{
		g = to_string(gold);
		gprefix = " ";
	}
	else
	{
		g = to_string(gold);
		gprefix = "";
	}

	string so;
	string soprefix;
	if (sonar / 10 == 0)
	{
		so = to_string(sonar);
		soprefix = " ";
	}
	else
	{
		so = to_string(sonar);
		soprefix = "";
	}

	string o;
	string oprefix;
	if (barrelsLeft / 10 == 0)
	{
		o = to_string(barrelsLeft);
		oprefix = " ";
	}
	else
	{
		o = to_string(barrelsLeft);
		oprefix = "";
	}

	string output = "Scr: " + scoreprefix + s + "  Lvl: " + levelprefix + levs + "  Lives: " + life + "  Hlth: " + hprefix + h + "%  Wtr: " + wprefix + w + "  Gld: " + gprefix + g + "  Sonar: " + soprefix + so + "  Oil Left: " + oprefix + o;

	return output;

}

void StudentWorld::spawnsquirt(int x, int y)
{
	if (p->isblocked(x, y) == false)
	{
		Squirt* sq = new Squirt(this, x, y, p->getDirection());
		m_objects.push_back(sq);
		m_num_objects++;
	}

	playSound(SOUND_PLAYER_SQUIRT);
}

bool StudentWorld::annoyprotester(int x, int y)
{
	bool success = false;
	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isProtester() == true) // Check if it is a protestor
			{
				if (m_objects[i]->beyonddistance(x, y, 3) == false)
				{
					m_objects[i]->dechitpoints(2);
					if (m_objects[i]->getStatus() == true)
					{
						playSound(SOUND_PROTESTER_GIVE_UP);
						m_objects[i]->settickcounter(0);
						increaseScore(100);
					}
					else
					{
						playSound(SOUND_PROTESTER_ANNOYED);
						m_objects[i]->setwait();
					}

					success = true;
				}
			}
		}
	}
	return success;
}

bool StudentWorld::pwithindistance(int x, int y)
{
	if (p->beyonddistance(x, y, 4) == false)
	{
		return true;
	}
	return false;
}

void StudentWorld::annoyplayer(int x, int y)
{
		p->dechitpoints(2);
		playSound(SOUND_PROTESTER_YELL);
		if (p->getStatus() == true)
		{
			playSound(SOUND_PLAYER_GIVE_UP);
		}

}

bool StudentWorld::checkifindir(int x, int y, char dir)
{
	if (dir == 'l')
	{
		if (p->getX() < x)
		{
			return true;
		}
	}
	if (dir == 'r')
	{
		if (p->getX() > x)
		{
			return true;
		}
	}
	if (dir == 'u')
	{
		if (p->getY() > y)
		{
			return true;
		}
	}
	if (dir == 'd')
	{
		if (p->getY() < y)
		{
			return true;
		}
	}
	return false;
}

bool StudentWorld::inlinewithplayer(int x, int y)
{
	if (p->getX() == x)
	{
		return true;
	}
	if (p->getY() == y)
	{
		return true;
	}
	return false;
}

bool StudentWorld::hasclearpath(int x, int y)
{
	if (p->getX() == x)
	{
		if (p->getY() > y)
		{
			for (int i = y; i <= p->getY(); i++)
			{
				if (earthExists(x, i) == true)
				{
					return false;
				}
				if (earthExists(x + 3, i) == true)
				{
					return false;
				}
				if (withinboulder(x, i) == true)
				{
					return false;
				}
				if (withinboulder(x + 3, i) == true)
				{
					return false;
				}
				
			}
		}
		if (p->getY() < y)
		{
			for (int i = p->getY(); i <= y; i++)
			{
				if (earthExists(x, i) == true)
				{
					return false;
				}
				if (earthExists(x + 3, i) == true)
				{
					return false;
				}
				if (withinboulder(x, i) == true)
				{
					return false;
				}
				if (withinboulder(x + 3, i) == true)
				{
					return false;
				}
			}
		}
	}
	else if (p->getY() == y)
	{
		if (p->getX() > x)
		{
			for (int i = x; i <= p->getX(); i++)
			{
				if (earthExists(i, y) == true)
				{
					return false;
				}
				if (earthExists(i, y + 3) == true)
				{
					return false;
				}
				if (withinboulder(i, y) == true)
				{
					return false;
				}
				if (withinboulder(i, y + 3) == true)
				{
					return false;
				}
			}
		}
		if (p->getX() < x)
		{
			for (int i = p->getX(); i <= x; i++)
			{
				if (earthExists(i, y) == true)
				{
					return false;
				}
				if (earthExists(i, y + 3) == true)
				{
					return false;
				}
				if (withinboulder(i, y) == true)
				{
					return false;
				}
				if (withinboulder(i, y + 3) == true)
				{
					return false;
				}
			}
		}
	}
	return true;
}

string StudentWorld::finddirection(int x, int y)
{
	if (p->getX() == x)
	{
		if (p->getY() > y)
		{
			return "up";
		}
		if (p->getY() < y)
		{
			return "down";
		}
	}
	else if (p->getY() == y)
	{
		if (p->getX() > x)
		{
			return "right";
		}
		if (p->getX() < x)
		{
			return "left";
		}
	}
}

bool StudentWorld::boulderdies(int x, int y)
{
	if (y == -1)
	{
		return true;
	}
	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isBoulder() == true) // Check if it is a boulder
			{
				for (int j = m_objects[i]->getX(); j < m_objects[i]->getX() + 4; j++)
				{
					if (j == x && m_objects[i]->getY() + 3 == y)
					{
						return true;
					}
					if (j == x + 3 && m_objects[i]->getY() + 3 == y)
					{
						return true;
					}
				}
			}
		}
	}
	for (int k = x; k < x + 4; k++)
	{
		if (earthExists(k, y) == true)
		{
			return true;
		}
	}

	return false;

}

void StudentWorld::bonk(int x, int y)
{
	if (p->beyonddistance(x, y, 3) == false)
	{
		p->dechitpoints(100);
		playSound(SOUND_PLAYER_GIVE_UP);
	}
	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isProtester() == true) // Check if it is a protestor
			{
				if (m_objects[i]->beyonddistance(x, y, 3) == false)
				{
					m_objects[i]->dechitpoints(100);
					playSound(SOUND_PROTESTER_GIVE_UP);
					m_objects[i]->settickcounter(0);
					increaseScore(500);

				}
			}
		}
	}
}

bool StudentWorld::withinboulder(int x, int y)
{

	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isBoulder() == true) // Check if it is a boulder
			{
				if (m_objects[i]->beyonddistance(x, y, 3) == false)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool StudentWorld::pwithinboulder(int x, int y)
{
	bool isinboulder = false;

	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isBoulder() == true) // Check if it is a boulder
			{
				// Check for all 16 squares that the TunnelMan occupies
				for (int k = m_objects[i]->getY(); k < m_objects[i]->getY() + 4; k++)
				{
					for (int j = m_objects[i]->getX(); j < m_objects[i]->getX() + 4; j++)
					{
						if (x == j && y == k)
						{
							isinboulder = true;
						}
						if (x + 3 == j && y == k)
						{
							isinboulder = true;
						}
						if (x == j && y + 3 == k)
						{
							isinboulder = true;
						}
						if (x + 3 == j && y + 3 == k)
						{
							isinboulder = true;

						}
					}
				}
			}
		}
	}
	return isinboulder;
}

void StudentWorld::sonarcharge(int x, int y)
{
	playSound(SOUND_SONAR);
	for (int i = 0; i < m_num_objects; i++)	//Loop through every object in the object vector
	{
		if (m_objects[i] != nullptr)
		{
			if (m_objects[i]->isGoodie() == true) // Check if it is a boulder
			{
				if (m_objects[i]->beyonddistance(x, y, 12) == false)
				{
					m_objects[i]->setVisible(true);
					m_objects[i]->changeVisible(true);
				}
			}
		}
	}




}
