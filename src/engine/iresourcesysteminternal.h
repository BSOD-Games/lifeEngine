//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IRESOURCESYSTEM_INTERNAL_H
#define IRESOURCESYSTEM_INTERNAL_H

#include "engine/iresourcesystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IResourceSystemInternal : public IResourceSystem
	{
	public:
		virtual bool			Initialize( IEngine* Engine ) = 0;

		virtual void			AddPath( const char* Path ) = 0;
		virtual void			RemovePath( UInt32_t Index ) = 0;
		virtual void			ClearPaths() = 0;

		virtual UInt32_t		GetCountPaths() const = 0;
		virtual const char*		GetPath( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_INTERNAL_H

