//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Frustum
	{
	public:
		void			Update( const Matrix4x4_t& Projection, const Matrix4x4_t& View );

		bool			IsVisible( const Vector3D_t& MinPosition, const Vector3D_t& MaxPosition ) const;
		bool			IsVisible( const Vector3DInt_t& MinPosition, const Vector3DInt_t& MaxPosition ) const;
		bool			IsVisible( const Vector3DInt_t& Position, float Radius ) const;

	private:
		void			NormalizePlanes();

		Vector4D_t		planes[ 6 ];
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !FRUSTUM_H
