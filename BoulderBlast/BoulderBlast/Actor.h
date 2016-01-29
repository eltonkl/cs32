#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

void convertDirectionToCoordinates(GraphObject::Direction dir, int& x, int& y);
GraphObject::Direction reverseDirection(GraphObject::Direction dir);
GraphObject::Direction getRandomDirection();

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = none)
		: _world(world), GraphObject(imageID, startX, startY, dir) {};
	virtual ~Actor() = 0 {};
	virtual void doSomething() = 0;

	int getHealth() const
	{
		return _health;
	}
	StudentWorld* getWorld() const
	{
		return _world;
	}
	bool isAlive() const
	{
		return _health > 0;
	}
	void setHealth(int health)
	{
		_health = health;
	}

	virtual void takeDamage(int damage) {};
	virtual bool doesBlockMovement() const = 0;
	virtual bool doesBlockBoulder() const = 0;
	virtual bool doesBlockBullet() const = 0;
	virtual bool isStealable() const = 0;
	virtual bool countsInFactoryCensus() const = 0;
	virtual bool canBeHitByBullet() const = 0;
	
private:
	StudentWorld* _world;
	int _health;
};

class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_BOULDER, startX, startY, none)
	{
		setHealth(10);
		setVisible(true);
	}
	virtual ~Boulder() {};
	virtual void doSomething() {};
	bool tryMoveInDirection(Direction dir);

	virtual void takeDamage(int damage)
	{
		setHealth(getHealth() - damage);
	}
	virtual bool doesBlockMovement() const
	{
		return true;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return true;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return true;
	}
};

class IShootAtPlayer
{
public:
	virtual ~IShootAtPlayer() = 0 {};
	void shoot(StudentWorld* world, GraphObject::Direction dir, int startX, int startY);
protected:
	bool tryShootAtPlayer(Actor* actor);
};

class Robot : public Actor
{
public: 
	Robot(StudentWorld* world, int imageID, int startX, int startY, Direction dir);
	virtual ~Robot() = 0 {};
	void doSomething();

	virtual bool doesBlockMovement() const
	{
		return true;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return true;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return true;
	}
protected:
	virtual void doWhenActive() = 0;
	bool tryMoveInDirection(Direction dir);
private:
	int _ticks;
	int _currentTick;
};

class KleptoBot : public Robot
{
public:
	KleptoBot(StudentWorld* world, int startX, int startY)
		: Robot(world, IID_KLEPTOBOT, startX, startY, right)
	{
		initialize();
	}
	virtual ~KleptoBot() {};
	virtual void takeDamage(int damage);

	virtual bool countsInFactoryCensus() const
	{
		return true;
	}

protected:
	KleptoBot::KleptoBot(StudentWorld* world, int imageID, int startX, int startY)
		: Robot(world, imageID, startX, startY, right)
	{
		initialize();
	}
	virtual void doWhenActive();
	void setDistance(int dist)
	{
		_distance = dist;
	}
	void setDistanceBeforeTurning(int dist)
	{
		_distanceBeforeTurning = dist;
	}
private:
	void initialize();

	int _distance;
	int _distanceBeforeTurning;
	Actor* _goodie;
};

class AngryKleptoBot : public KleptoBot, public IShootAtPlayer
{
public:
	AngryKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~AngryKleptoBot() {};
	virtual void takeDamage(int damage);

protected:
	virtual void doWhenActive();
};

class Player : public Actor
{
public:
	Player(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_PLAYER, startX, startY, right), _ammo(20)
	{
		setHealth(20);
		setVisible(true);
	}
	virtual ~Player() {};
	void shoot(Direction dir, int startX, int startY);
	virtual void doSomething();
	virtual void takeDamage(int damage);
	void addAmmo(int ammo)
	{
		_ammo += ammo;
	}

	virtual bool doesBlockMovement() const
	{
		return true;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return true;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return true;
	}

	int getAmmo() const;
private:
	bool tryMoveInDirection(Direction dir);

	int _ammo;
};

class SnarlBot : public Robot, public IShootAtPlayer
{
public:
	SnarlBot(StudentWorld* world, int startX, int startY, Direction dir);
	virtual ~SnarlBot() {};
	virtual void takeDamage(int damage);

	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
protected:
	virtual void doWhenActive();
};

class Wall : public Actor
{
public:
	Wall(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_WALL, startX, startY)
	{
		setHealth(1);
		setVisible(true);
	}
	virtual ~Wall() {};
	virtual void doSomething() {};

	virtual bool doesBlockMovement() const
	{
		return true;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return true;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return true;
	}
};

class Bullet : public Actor
{
public:
	Bullet(StudentWorld* world, int startX, int startY, Direction dir)
		: Actor(world, IID_BULLET, startX, startY, dir)
	{
		setHealth(1);
		setVisible(true);
	}
	virtual ~Bullet() {};
	virtual void doSomething();

	virtual bool doesBlockMovement() const
	{
		return false;
	}
	virtual bool doesBlockBoulder() const
	{
		return false;
	}
	virtual bool doesBlockBullet() const
	{
		return false;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return false;
	}
private:
	void doForCurrentSquare();
};

class Hole : public Actor
{
public:
	Hole(StudentWorld* world, int startX, int startY)
		: Actor(world, IID_HOLE, startX, startY)
	{
		setHealth(1);
		setVisible(true);
	}
	virtual ~Hole() {};
	virtual void doSomething();

	virtual bool doesBlockMovement() const
	{
		return true;
	}
	virtual bool doesBlockBoulder() const
	{
		return false;
	}
	virtual bool doesBlockBullet() const
	{
		return false;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return false;
	}
};

class Exit : public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY) 
		: Actor(world, IID_EXIT, startX, startY)
	{
		setHealth(1);
	}
	virtual ~Exit() {};
	virtual void doSomething();

	virtual bool doesBlockMovement() const
	{
		return false;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return false;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return false;
	}
};

class Pickupable : public Actor
{
public:
	Pickupable(StudentWorld* world, int imageID, int startX, int startY)
		: Actor(world, imageID, startX, startY)
	{
		setHealth(1);
		setVisible(true);
	}
	virtual ~Pickupable() = 0 {};
	virtual void doSomething();

	virtual bool doesBlockMovement() const
	{
		return false;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return false;
	}
	virtual bool isStealable() const
	{
		return true;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return false;
	}
protected:
	virtual void giveBonus() = 0;
};

class Jewel : public Pickupable
{
public:
	Jewel(StudentWorld* world, int startX, int startY)
		: Pickupable(world, IID_JEWEL, startX, startY) {};
	virtual ~Jewel() {};

	virtual bool isStealable() const
	{
		return false;
	}
protected:
	virtual void giveBonus();
};

class ExtraLifeGoodie : public Pickupable
{
public:
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
		: Pickupable(world, IID_EXTRA_LIFE, startX, startY) {};
	virtual ~ExtraLifeGoodie() {};

protected:
	virtual void giveBonus();
};

class RestoreHealthGoodie : public Pickupable
{
public:
	RestoreHealthGoodie(StudentWorld* world, int startX, int startY)
		: Pickupable(world, IID_RESTORE_HEALTH, startX, startY) {};
	virtual ~RestoreHealthGoodie() {};

protected:
	virtual void giveBonus();
};

class AmmoGoodie : public Pickupable
{
public:
	AmmoGoodie(StudentWorld* world, int startX, int startY)
		: Pickupable(world, IID_AMMO, startX, startY) {};
	virtual ~AmmoGoodie() {};

protected:
	virtual void giveBonus();
};

class KleptoBotFactory : public Actor
{
public:
	KleptoBotFactory(StudentWorld* world, int startX, int startY, bool angry);
	virtual ~KleptoBotFactory() {};
	virtual void doSomething();

	virtual bool doesBlockMovement() const
	{
		return true;
	}
	virtual bool doesBlockBoulder() const
	{
		return true;
	}
	virtual bool doesBlockBullet() const
	{
		return true;
	}
	virtual bool isStealable() const
	{
		return false;
	}
	virtual bool countsInFactoryCensus() const
	{
		return false;
	}
	virtual bool canBeHitByBullet() const
	{
		return true;
	}

private:
	bool _angry;
	int _xmin, _xmax, _ymin, _ymax;
};
#endif // ACTOR_H_
