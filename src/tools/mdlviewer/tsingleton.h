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
	TSingleton()
	{
		LIFEENGINE_ASSERT( !singleton );
		singleton = static_cast< T* >( this );
	}

	virtual ~TSingleton()
	{
		singleton = nullptr;
	}

	static T*			GetInstance()
	{
		LIFEENGINE_ASSERT( singleton );
		return singleton;
	}

protected:
	static T*		singleton;

private:
	TSingleton( const TSingleton<T>& ) {}
	TSingleton& operator=( const TSingleton<T>& ) {}
};

//-------------------------------------------------------------------------//

template<class T>
T*				TSingleton<T>::singleton = nullptr;

//-------------------------------------------------------------------------//

#endif // !TSINGLETON_H
