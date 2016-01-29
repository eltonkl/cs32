#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void convertDirectionToCoordinates(GraphObject::Direction dir, int& x, int& y)
{
	switch (dir)
	{
	case GraphObject::left:
		x -= 1;
		break;
	case GraphObject::right:
		x += 1;
		break;
	case GraphObject::down:
		y -= 1;
		break;
	case GraphObject::up:
		y += 1;
		break;
	}
}

GraphObject::Direction reverseDirection(GraphObject::Direction dir)
{
	switch (dir)
	{
	case GraphObject::left:
		return GraphObject::right;
	case GraphObject::right:
		return GraphObject::left;
	case GraphObject::down:
		return GraphObject::up;
	case GraphObject::up:
		return GraphObject::down;
	case GraphObject::none:
		return GraphObject::none;
	}
	return GraphObject::none;
}

GraphObject::Direction getRandomDirection()
{
	switch (std::rand() % 4)
	{
	case 0:
		return GraphObject::left;
	case 1:
		return GraphObject::right;
	case 2:
		return GraphObject::up;
	case 3:
		return GraphObject::down;
	}
	return GraphObject::none;
}

bool Boulder::tryMoveInDirection(Direction dir)
{
	int x = getX();
	int y = getY();
	convertDirectionToCoordinates(dir, x, y);

	std::list<Actor*> actors = getWorld()->getActorsAtXY(x, y);
	if (actors.empty())
	{
		moveTo(x, y);
		return true;
	}
	for (auto& actor : actors)
	{
		if (dynamic_cast<Hole*>(actor) != nullptr)
		{
			moveTo(x, y);
			return true;
		}
	}
	return false;
}

void IShootAtPlayer::shoot(StudentWorld* world, GraphObject::Direction dir, int startX, int startY)
{
	world->addActor(new Bullet(world, startX, startY, dir));
	world->playSound(SOUND_ENEMY_FIRE);
}

bool IShootAtPlayer::tryShootAtPlayer(Actor* actor)
{
	int x = actor->getX();
	int y = actor->getY();
	int playerX = actor->getWorld()->getPlayer()->getX();
	int playerY = actor->getWorld()->getPlayer()->getY();
	bool checkObstructions = false;
	bool canFire = false;

	switch (actor->getDirection())
	{
	case GraphObject::left:
		if (playerY == y && playerX < x)
			checkObstructions = true;
		break;
	case GraphObject::right:
		if (playerY == y && playerX > x)
			checkObstructions = true;
		break;
	case GraphObject::down:
		if (playerX == x && playerY < y)
			checkObstructions = true;
		break;
	case GraphObject::up:
		if (playerX == x && playerY > y)
			checkObstructions = true;
		break;
	}
	if (checkObstructions)
	{
		canFire = true;
		while (x != playerX || y != playerY)
		{
			convertDirectionToCoordinates(actor->getDirection(), x, y);
			auto actors = actor->getWorld()->getActorsAtXY(x, y);
			for (auto& actor : actors)
			{
				if (actor->doesBlockBullet())
				{
					canFire = false;
					break;
				}
			}
		}
	}
	if (canFire)
	{
		int x = actor->getX();
		int y = actor->getY();
		convertDirectionToCoordinates(actor->getDirection(), x, y);
		shoot(actor->getWorld(), actor->getDirection(), x, y);
		return true;
	}
	return false;
}

Robot::Robot(StudentWorld* world, int imageID, int startX, int startY, Direction dir)
	: Actor(world, imageID, startX, startY, dir)
{
	_currentTick = 0;

	_ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
	// level number (0, 1, 2, etc.)
	if (_ticks < 3)
		_ticks = 3; // no robot moves more frequently than this
};

void Robot::doSomething()
{
	if (!isAlive())
		return;

	if (_currentTick == _ticks)
	{
		_currentTick = 0;
		doWhenActive();
	}
	else
		_currentTick++;
}

bool Robot::tryMoveInDirection(Direction dir)
{
	int x = getX();
	int y = getY();
	convertDirectionToCoordinates(dir, x, y);
	auto actors = getWorld()->getActorsAtXY(x, y);
	bool obstructed = false;
	if (getWorld()->isPlayerAtXY(x, y))
		obstructed = true;
	else
	{
		for (auto& actor : actors)
		{
			if (actor->doesBlockMovement())
			{
				obstructed = true;
				break;
			}
		}
	}
	if (!obstructed)
	{
		setDirection(dir);
		moveTo(x, y);
		return true;
	}
	return false;
}

void KleptoBot::initialize()
{
	setHealth(5);
	setVisible(true);
	setDistance(0);
	setDistanceBeforeTurning(std::rand() % 6 + 1);
	_goodie = nullptr;
}

void KleptoBot::doWhenActive()
{
	if (_goodie == nullptr)
	{
		auto actors = getWorld()->getActorsAtXY(getX(), getY());
		for (auto& actor : actors)
		{
			if (actor->isStealable())
			{
				if (std::rand() % 10 == 0)
				{
					_goodie = actor;
					_goodie->setVisible(false);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					return;
				}
			}
		}
	}
	if (_distance < _distanceBeforeTurning)
	{
		if (tryMoveInDirection(getDirection()))
		{
			_distance++;
			if (_goodie != nullptr)
			{
				_goodie->moveTo(getX(), getY());
			}
			return;
		}
	}
	_distanceBeforeTurning = std::rand() % 6 + 1;
	Direction dir[4] = { getRandomDirection(), getRandomDirection(), getRandomDirection(), getRandomDirection() };
	while (dir[0] == dir[1])
		dir[1] = getRandomDirection();
	while (dir[2] == dir[1] || dir[2] == dir[0])
		dir[2] = getRandomDirection();
	while (dir[3] == dir[2] || dir[3] == dir[1] || dir[3] == dir[0])
		dir[3] = getRandomDirection();
	for (int i = 0; i < 4; i++)
	{
		if (tryMoveInDirection(dir[i]))
		{
			_distance = 1;
			if (_goodie != nullptr)
			{
				_goodie->moveTo(getX(), getY());
			}
			return;
		}
	}
	setDirection(dir[0]);
	_distance = 0;
}

void KleptoBot::takeDamage(int damage)
{
	setHealth(getHealth() - damage);

	if (getHealth() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else
	{
		if (_goodie != nullptr)
		{
			_goodie->moveTo(getX(), getY());
			_goodie->setVisible(true);
		}
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(10);
	}
}

AngryKleptoBot::AngryKleptoBot(StudentWorld* world, int startX, int startY)
	: KleptoBot(world, IID_ANGRY_KLEPTOBOT, startX, startY)
{
	setHealth(8);
}

void AngryKleptoBot::takeDamage(int damage)
{
	KleptoBot::takeDamage(damage);
	if (!isAlive())
		getWorld()->increaseScore(10);
}

void AngryKleptoBot::doWhenActive()
{
	if (!tryShootAtPlayer(this))
		KleptoBot::doWhenActive();
}

void Player::doSomething()
{
	if (!isAlive())
		return;

	int ch;
	if (getWorld()->getKey(ch))
	{
		int x = getX();
		int y = getY();
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			tryMoveInDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			tryMoveInDirection(right);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			tryMoveInDirection(down);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			tryMoveInDirection(up);
			break;
		case KEY_PRESS_ESCAPE:
			setHealth(0);
			break;
		case KEY_PRESS_SPACE:
			convertDirectionToCoordinates(getDirection(), x, y);
			shoot(getDirection(), x, y);
			break;
		}
	}
}

void Player::shoot(Direction dir, int startX, int startY)
{
	if (_ammo > 0)
	{
		_ammo--;
		getWorld()->addActor(new Bullet(getWorld(), startX, startY, getDirection()));
		getWorld()->playSound(SOUND_PLAYER_FIRE);
	}
}

void Player::takeDamage(int damage)
{
	setHealth(getHealth() - damage);
	if (getHealth() > 0)
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
	else
		getWorld()->playSound(SOUND_PLAYER_DIE);
}

int Player::getAmmo() const
{
	return _ammo;
}

bool Player::tryMoveInDirection(Direction dir)
{
	int x = getX();
	int y = getY();
	convertDirectionToCoordinates(dir, x, y);

	std::list<Actor*> actors = getWorld()->getActorsAtXY(x, y);
	if (actors.empty())
	{
		moveTo(x, y);
		return true;
	}
	for (auto& actor : actors)
	{
		if (dynamic_cast<Boulder*>(actor) != nullptr)
		{
			bool success = static_cast<Boulder*>(actor)->tryMoveInDirection(getDirection());
			if (!success)
				return false;
			else
			{
				moveTo(x, y);
				return true;
			}
		}
		if (actor->doesBlockMovement())
			return false;
	}
	
	moveTo(x, y);
	return true;
}

SnarlBot::SnarlBot(StudentWorld* world, int startX, int startY, Direction dir) 
	: Robot(world, IID_SNARLBOT, startX, startY, dir)
{
	setHealth(10);
	setVisible(true);
}

void SnarlBot::doWhenActive()
{
	if (!tryShootAtPlayer(this))
	{
		if (!tryMoveInDirection(getDirection()))
		{
			setDirection(reverseDirection(getDirection()));
		}
	}
}

void SnarlBot::takeDamage(int damage)
{
	setHealth(getHealth() - damage);
	if (getHealth() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(100);
	}
}

void Bullet::doSomething()
{
	if (!isAlive())
		return;

	doForCurrentSquare();
	if (!isAlive())
		return;

	int x = getX();
	int y = getY();
	convertDirectionToCoordinates(getDirection(), x, y);
	moveTo(x, y);
	doForCurrentSquare();
}

void Bullet::doForCurrentSquare()
{
	int x = getX();
	int y = getY();
	std::list<Actor*> actors = getWorld()->getActorsAtXY(x, y);
	if (getWorld()->isPlayerAtXY(x, y))
		actors.push_back(getWorld()->getPlayer());
	for (auto& actor : actors)
	{
		if (actor->canBeHitByBullet())
		{
			actor->takeDamage(2);
			setHealth(0);
			return;
		}
	}
		
	return;
}

void Hole::doSomething()
{
	if (!isAlive())
		return;

	std::list<Actor*> actors = getWorld()->getActorsAtXY(getX(), getY());
	for (auto& actor : actors)
	{
		if (dynamic_cast<Boulder*>(actor) != nullptr)
		{
			setHealth(0);
			actor->takeDamage(actor->getHealth());
			return;
		}
	}
}

void Exit::doSomething()
{
	if (isVisible() && getWorld()->isPlayerAtXY(getX(), getY()))
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000 + getWorld()->getBonus());
		getWorld()->signalCompletion();
	}
}

void Pickupable::doSomething()
{
	if (!isAlive())
		return;

	if (getWorld()->isPlayerAtXY(getX(), getY()) && isVisible())
	{
		setHealth(0);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		giveBonus();
	}
}

void Jewel::giveBonus()
{
	getWorld()->increaseScore(50);
	getWorld()->decJewels();
}

void ExtraLifeGoodie::giveBonus()
{
	getWorld()->increaseScore(1000);
	getWorld()->incLives();
}

void RestoreHealthGoodie::giveBonus()
{
	getWorld()->increaseScore(500);
	getWorld()->getPlayer()->setHealth(20);
}

void AmmoGoodie::giveBonus()
{
	getWorld()->increaseScore(100);
	getWorld()->getPlayer()->addAmmo(20);
}

KleptoBotFactory::KleptoBotFactory(StudentWorld* world, int startX, int startY, bool angry)
	: Actor(world, IID_ROBOT_FACTORY, startX, startY), _angry(angry)
{
	setHealth(1);
	setVisible(true);
	_xmin = getX() - 3;
	_xmax = getX() + 3;
	_ymin = getY() - 3;
	_ymax = getY() + 3;

	if (_xmin < 0)
		_xmin = 0;
	if (_xmax > VIEW_WIDTH)
		_xmax = VIEW_WIDTH - 1;
	if (_ymin < 0)
		_ymin = 0;
	if (_ymax > VIEW_HEIGHT)
		_ymax = VIEW_HEIGHT - 1;
}

void KleptoBotFactory::doSomething()
{
	int count = 0;
	for (int x = _xmin; x <= _xmax; x++)
		for (int y = _ymin; y <= _ymax; y++)
		{
			std::list<Actor*> actors = getWorld()->getActorsAtXY(x, y);
			for (auto& actor : actors)
			{
				if (actor->countsInFactoryCensus())
					count++;
			}
		}
	if (count < 3)
	{
		std::list<Actor*> actors = getWorld()->getActorsAtXY(getX(), getY());
		bool robotOnFactory = false;
		for (auto& actor : actors)
		{
			if (actor->countsInFactoryCensus())
				robotOnFactory = true;
		}
		if (!robotOnFactory)
		{
			if (std::rand() % 50 == 0)
			{
				Actor* actor;
				if (_angry)
					actor = new AngryKleptoBot(getWorld(), getX(), getY());
				else
					actor = new KleptoBot(getWorld(), getX(), getY());
				getWorld()->addActor(actor);
				getWorld()->playSound(SOUND_ROBOT_BORN);
			}
		}
	}
}