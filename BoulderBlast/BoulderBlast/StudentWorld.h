#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	void addActor(Actor* actor);
	
	unsigned int getBonus() const;
	std::list<Actor*> getActorsAtXY(int X, int Y) const;
	bool isPlayerAtXY(int X, int Y) const;

	Player* getPlayer() const
	{
		return _player;
	}
	void signalCompletion()
	{
		_complete = true;
	}
	void decJewels()
	{
		_jewels--;
	}
private:
	int loadLevel(std::string filename);
	void updateDisplayText();
	
	std::list<Actor*> _actors;
	std::list<Actor*> _actorsToAdd;
	Player* _player;
	unsigned int _bonus;
	int _jewels;
	bool _exitRevealed;
	bool _complete;
};

#endif // STUDENTWORLD_H_
