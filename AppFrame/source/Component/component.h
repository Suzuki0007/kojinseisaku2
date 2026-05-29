#pragma once
#include <concepts>

template <typename TOwner>
class Component
{
public:
	explicit Component(TOwner& owner) : _owner(owner) {}
	virtual ~Component() = default;

	virtual void Update([[maybe_unused]] float deltaTime) {}
	virtual void Render() {}

protected:
	TOwner& _owner;
};

