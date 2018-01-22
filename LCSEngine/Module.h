#ifndef __MODULE_H__
#define __MODULE_H__

class Application;

class Module
{
public:

	Module(bool active = true) : active(active) {}

	~Module() {}

	bool isEnabled() const
	{
		return active;
	}

	bool enable()
	{
		if (active == false)
		{
			return active = start();
		}

		return true;
	}

	bool disable()
	{
		if (active == true)
		{
			return active = !cleanUp();
		}

		return true;
	}

	virtual bool init()
	{
		return true;
	}

	virtual bool start()
	{
		return true;
	}

	virtual update_status preUpdate(const float deltaTime)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status update(const float deltaTime)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status postUpdate(const float deltaTime)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool cleanUp()
	{
		return true;
	}

private:
	bool active = false;
};

#endif // __MODULE_H__