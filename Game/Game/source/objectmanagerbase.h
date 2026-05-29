#pragma once
#include <memory>
#include <string>
#include "objectfactory.h"

template<typename T, typename Factory = ObjectFactory<T>>
class ObjectManagerBase
{
public:
	using Creator = typename Factory::Creator;
	void RegisterType(const std::string& type, Creator creator)
	{
		_factory.RegisterType(type, std::move(creator));
	}

	void Create(const std::string& type)
	{
		_object = _factory.Create(type);
	}

	T* Get() const { return _object.get(); }
	std::unique_ptr<T>& GetOwner() { return _object; }

protected:
	ObjectManagerBase() = default;
	virtual ~ObjectManagerBase() = default;
	Factory _factory;// オブジェクトのファクトリー
	std::unique_ptr<T> _object;// 管理するオブジェクト
};

