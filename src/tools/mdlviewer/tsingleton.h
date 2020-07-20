//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TSINGLETON_H
#define TSINGLETON_H

#include "engine/lifeengine.h"

//-------------------------------------------------------------------------//

template<class T>
class TSingleton
{
public:
	TSingleton()			{}
	virtual ~TSingleton()	{}

	static T*			GetInstance()
	{
		static T*			instance = nullptr;
		if ( !instance )	instance = new T();

		return instance;
	}

private:
	TSingleton( const TSingleton<T>& ) {}
	TSingleton& operator=( const TSingleton<T>& ) {}
};

//-------------------------------------------------------------------------//

#endif // !TSINGLETON_H
