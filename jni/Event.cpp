#include "Event.hpp"

namespace XPG
{
	Event::Event()
	{
		_target = 0;
		_userData = 0;
	}

	Event::~Event()
	{
	}

	void Event::Aim(Target target, void* userData)
	{
		_target = target;
		_userData = userData;
	}

	void Event::Fire(Ammo& ammo) const
	{
		if (_target)
		{
			ammo.userData = _userData;
			_target(ammo);
		}
	}
}
