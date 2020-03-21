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
        // IReferenceObject
        virtual void                    IncrementReference();
        virtual void                    DecrementReference();
        virtual void                    Release();
        virtual UInt32_t                GetCountReferences() const;

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

		inline const Matrix4x4_t&		GetTransformation()
		{
			if ( isNeadUpdateTransformation )
			{
				transformation = glm::translate( position );
				isNeadUpdateTransformation = false;
			}
			
			return transformation;
		}

	private:
		bool			isNeadUpdateTransformation;
		
		float			intensivity;
		float			radius;

		Matrix4x4_t		transformation;
		Vector3D_t		position;
		Vector4D_t		color;
		Vector4D_t		specular;
        UInt32_t        countReferences;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !POINTLIGHT_H

