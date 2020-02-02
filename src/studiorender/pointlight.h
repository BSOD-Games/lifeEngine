//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "studiorender/ipointlight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class PointLight : public IPointLight
	{
	public:
		// IPointLight
		virtual void					SetPosition( const Vector3D_t& Position );
		virtual void					SetRadius( float Radius );

		virtual const Vector3D_t&		GetPosition() const;
		virtual float					GetRadius() const;

		// ILight
		virtual void					SetColor( const Vector4D_t& Color );
		virtual void					SetSpecular( const Vector4D_t& Color );
		virtual void					SetIntensivity( float Intensivity );

		virtual const Vector4D_t&		GetColor() const;
		virtual const Vector4D_t&		GetSpecular() const;
		virtual float					GetIntensivity() const;

		// PointLight
		PointLight();
		~PointLight();

	private:
		float			intensivity;
		float			radius;

		Vector3D_t		position;
		Vector4D_t		color;
		Vector4D_t		specular;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !POINTLIGHT_H

