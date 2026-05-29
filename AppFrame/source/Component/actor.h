#pragma once
#include "componentowner.h"

class Actor
{
public:
	Actor() = default;
	virtual ~Actor() = default;
	
	template <class T, class ... Args>
	T* Add(Args&& ... args)
	{
		return _componentOwner.AddComponent<T>(*this, std::forward<Args>(args)...);
	}

	void Update(float deltaTime)
	{
		_componentOwner.Update(deltaTime);
	}

	void Render()
	{
		_componentOwner.Render();
	}

	void Clear()
	{
		_componentOwner.Clear();
	}

private:
	ComponentOwner<Actor> _componentOwner;
};

