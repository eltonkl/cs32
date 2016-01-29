#include "StudentWorld.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{
	_player = nullptr;
}

int StudentWorld::init()
{
	_bonus = 1000;
	_jewels = 0;
	_exitRevealed = false;
	_complete = false;

	if (getLevel() > 99)
		return GWSTATUS_PLAYER_WON;
	ostringstream level;
	level.fill('0');
	level << "level" << setw(2) << getLevel() << ".dat";
	int result = loadLevel(level.str());
	if (result == -1)
		return GWSTATUS_PLAYER_WON;
	else if (result == -2)
		return GWSTATUS_LEVEL_ERROR;
	
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	updateDisplayText();

	if (_jewels == 0 && !_exitRevealed)
	{
		playSound(SOUND_REVEAL_EXIT);
		list<Actor*>::iterator it = find_if(_actors.begin(), _actors.end(), [](Actor* actor)
		{
			return dynamic_cast<Exit*>(actor) != nullptr;
		});
		(*it)->setVisible(true);
		_exitRevealed = true;
	}

	for (auto& actor : _actors)
	{
		if (actor->isAlive())
		{
			actor->doSomething();
			if (!_player->isAlive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (_complete)
				return GWSTATUS_FINISHED_LEVEL;
		}
	}

	_player->doSomething();

	list<Actor*>::iterator it;
	while (true)
	{
		it = find_if(_actors.begin(), _actors.end(), [](Actor* actor)
		{
			return !actor->isAlive();
		});
		if (it == _actors.end())
			break;
		else
		{
			delete *it;
			_actors.remove(*it);
		}
	}

	while (!_actorsToAdd.empty())
	{
		it = _actorsToAdd.begin();
		_actors.push_back(*it);
		_actorsToAdd.remove(*it);
	}

	if(_bonus > 0)
		_bonus--;

	if (!_player->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (_player != nullptr)
		delete _player;
	for (auto& actor : _actors)
		delete actor;
	_actors.clear();
}

void StudentWorld::addActor(Actor* actor)
{
	if (actor != nullptr)
		_actorsToAdd.push_back(actor);
}

unsigned int StudentWorld::getBonus() const
{
	return _bonus;
}

list<Actor*> StudentWorld::getActorsAtXY(int X, int Y) const
{
	list<Actor*> actors;
	for (auto& actor : _actors)
	{
		if (actor->getX() == X && actor->getY() == Y)
			actors.push_back(actor);
	}
	return actors;
}

bool StudentWorld::isPlayerAtXY(int X, int Y) const
{
	if (_player->getX() == X && _player->getY() == Y)
		return true;
	else
		return false;
}

//Return value of -2: bad format
//Return value of -1: file not found
//Return value of 0: success
int StudentWorld::loadLevel(string filename)
{
	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(filename);
	if (result == Level::load_fail_bad_format)
		return -2;
	else if (result == Level::load_fail_file_not_found)
		return -1;

	for (int y = 0; y < VIEW_HEIGHT; y++)
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			Level::MazeEntry entry = lev.getContentsOf(x, y);
			switch (entry)
			{
			case Level::player:
				_player = new Player(this, x, y);
				break;
			case Level::wall:
				_actors.push_back(new Wall(this, x, y));
				break;
			case Level::boulder:
				_actors.push_back(new Boulder(this, x, y));
				break;
			case Level::hole:
				_actors.push_back(new Hole(this, x, y));
				break;
			case Level::jewel:
				_actors.push_back(new Jewel(this, x, y));
				_jewels++;
				break;
			case Level::exit:
				_actors.push_back(new Exit(this, x, y));
				break;
			case Level::extra_life:
				_actors.push_back(new ExtraLifeGoodie(this, x, y));
				break;
			case Level::restore_health:
				_actors.push_back(new RestoreHealthGoodie(this, x, y));
				break;
			case Level::ammo:
				_actors.push_back(new AmmoGoodie(this, x, y));
				break;
			case Level::vert_snarlbot:
				_actors.push_back(new SnarlBot(this, x, y, GraphObject::down));
				break;
			case Level::horiz_snarlbot:
				_actors.push_back(new SnarlBot(this, x, y, GraphObject::right));
				break;
			case Level::kleptobot_factory:
				_actors.push_back(new KleptoBotFactory(this, x, y, false));
				break;
			case Level::angry_kleptobot_factory:
				_actors.push_back(new KleptoBotFactory(this, x, y, true));
				break;
			}
		}

	return 0;
}

void StudentWorld::updateDisplayText()
{
	ostringstream ss;
	ss.fill('0');
	ss << "Score: " << setw(7) << getScore() << "  ";
	ss << "Level: " << setw(2) << getLevel() << "  ";
	ss.fill(' ');
	ss << "Lives: " << setw(2) << getLives() << "  ";
	ss.setf(ios::fixed);
	ss.precision(0);
	ss << "Health: " << setw(3) << (static_cast<float>(_player->getHealth()) / 20.0) * 100 << "%  ";
	ss << "Ammo: " << setw(3) << _player->getAmmo() << "  ";
	ss << "Bonus: " << setw(4) << _bonus;
	setGameStatText(ss.str());
}