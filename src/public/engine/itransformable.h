//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ITRANSFORMABLE_H
#define ITRANSFORMABLE_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ITransformable
	{
	public:
        virtual ~ITransformable() {}
		virtual void					Move( const Vector3D_t& FactorMove ) = 0;
		virtual void					Rotate( const Vector3D_t& FactorRotate ) = 0;
		virtual void					Rotate( const Quaternion_t& FactorRotate ) = 0;
		virtual void					Scale( const Vector3D_t& FactorScale ) = 0;

		virtual void					SetPosition( const Vector3D_t& Position ) = 0;
		virtual void					SetRotation( const Vector3D_t& Rotation ) = 0;
		virtual void					SetRotation( const Quaternion_t& Rotation ) = 0;
		virtual void					SetScale( const Vector3D_t& Scale ) = 0;

		virtual const Vector3D_t&		GetPosition() const = 0;
		virtual const Quaternion_t&		GetRotation() const = 0;
		virtual const Vector3D_t&		GetScale() const = 0;
		virtual const Matrix4x4_t&		GetTransformation() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ITRANSFORMABLE_H

