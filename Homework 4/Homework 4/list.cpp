void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (path != "")
		path += '/';
	if (m->menuItems() != nullptr)
	{
		if (m->name() != "")
			cout << path << m->name() << endl;
		for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
		{
			listAll(*it, path + m->name());
		}
	}
	else
	{
		cout << path << m->name() << endl;
	}
}