// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef SINGLETON_H
#define SINGLETON_H

#include "Misc/CoreDefines.h"

namespace le
{
	template< typename T >
	class TSingleton
	{
	public:
		/* Get instance */
		FORCEINLINE static T* GetInstance()
		{
			if ( !instance )	instance = new T();
			return instance;
		}

	protected:
		/* Constructor */
		TSingleton() {}

		/* Constructor copy */
		TSingleton( const TSingleton& Copy ) {}

		/* Destructor */
		virtual ~TSingleton() {}

		/* Operator = */
		FORCEINLINE const TSingleton& operator=( const TSingleton& Right )
		{
			return *this;
		}

		static T*		instance;
	};

	template< typename T > T*		TSingleton<T>::instance = nullptr;
}

#endif // !SINGLETON_H
