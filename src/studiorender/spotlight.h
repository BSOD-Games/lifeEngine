//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "studiorender/ispotlight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class SpotLight : public ISpotLight
	{
	public:
		// ISpotLight
		virtual void						SetPosition( const Vector3D_t& Position );
		virtual void						SetRotation( const Quaternion_t& Quaternion );
		virtual void						SetRadius( float Radius );
		virtual void						SetHeight( float Height );

		virtual const Vector3D_t&			GetPosition() const;
		virtual const Quaternion_t&			GetRotation() const;
		virtual float						GetRadius() const;
		virtual float						GetHeight() const;

		// ILight
		virtual void						SetColor( const Vector4D_t& Color );
		virtual void						SetSpecular( const Vector4D_t& Color );
		virtual void						SetIntensivity( float Intensivity );

		virtual const Vector4D_t&			GetColor() const;
		virtual const Vector4D_t&			GetSpecular() const;
		virtual float						GetIntensivity() const;

		// SpotLight
		SpotLight();
		~SpotLight();

		inline float						GetCutoff()
		{
			if ( isNeadUpdateCutoff )
			{
				cutoff = height / ( sqrt( pow( height, 2.f ) + pow( radius, 2 ) ) );
				isNeadUpdateCutoff = false;
			}

			return cutoff;
		}

		inline const Matrix4x4_t&			GetTransformation()
		{
			if ( isNeadUpdateTransformation )
			{
				transformation = glm::translate( position ) * glm::mat4_cast( rotation );
				isNeadUpdateTransformation = false;
			}

			return transformation;
		}

		inline Vector3D_t					GetDirection()
		{
			if ( isNeadUpdataDirection )
			{
				direction = glm::normalize( rotation * Vector3D_t( 0.f, -1.f, 0.f ) );
				isNeadUpdataDirection = false;
			}

			return direction;
		}

	private:
		bool			isNeadUpdateCutoff;
		bool			isNeadUpdateTransformation;
		bool			isNeadUpdataDirection;

		float			intensivity;
		float			radius;
		float			height;
		float			cutoff;

		Matrix4x4_t		transformation;
		Vector3D_t		position;
		Vector3D_t		direction;
		Quaternion_t	rotation;
		Vector4D_t		color;
		Vector4D_t		specular;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SPOTLIGHT_H

