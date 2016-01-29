class Character
{
public:
	Character(string name) : _name(name) {};
	virtual ~Character() = 0 {};
	string name() const
	{
		return _name;
	}
	virtual void printWeapon() const = 0;
	virtual string attackAction() const
	{
		return "rushes toward the enemy";
	}
private:
	string _name;
};

class Dwarf : public Character
{
public:
	Dwarf(string name) : Character(name) {};
	virtual ~Dwarf()
	{
		cout << "Destroying " << name() << " the dwarf" << endl;
	}
	virtual void printWeapon() const
	{
		cout << "an axe";
	}
};

class Elf : public Character
{
public:
	Elf(string name, int arrows) : Character(name), _arrows(arrows) {};
	virtual ~Elf()
	{
		cout << "Destroying " << name() << " the elf" << endl;
	}
	virtual void printWeapon() const
	{
		cout << "a bow and a quiver of " << _arrows << " arrows";
	}
private:
	int _arrows;
};

class Boggie : public Character
{
public:
	Boggie(string name) : Character(name) {};
	virtual ~Boggie()
	{
		cout << "Destroying " << name() << " the boggie" << endl;
	}
	virtual void printWeapon() const
	{
		cout << "a short sword";
	}
	virtual string attackAction() const
	{
		return "whimpers";
	}
};