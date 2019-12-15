//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://gitlab.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef RAY_H
#define RAY_H

#include "common/types.h"

//-------------------------------------------------------------------------//

namespace le
{
	//-------------------------------------------------------------------------//
	
	struct Ray
	{
		Ray( const Vector3D_t& Origin = Vector3D_t( 0.f ), const Vector3D_t& Direction = Vector3D_t( 0.f ) ) :
			origin( Origin ),
			direction( Direction )
		{}

		inline void					Set( const Vector3D_t& Origin, const Vector3D_t& Direction )
		{
			origin = Origin;
			direction = Direction;
		}

		Vector3D_t		origin;		
		Vector3D_t		direction;	
	};

	//-------------------------------------------------------------------------//
}

//-------------------------------------------------------------------------//

#endif // !RAY_H
