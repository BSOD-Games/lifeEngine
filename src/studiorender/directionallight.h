//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "studiorender/idirectionallight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class DirectionalLight : public IDirectionalLight
	{
	public:
        // IReferenceObject
        virtual void                        IncrementReference();
        virtual void                        DecrementReference();
        virtual void                        Release();
        virtual UInt32_t                    GetCountReferences() const;

		// IDirectionalLight
		virtual void						SetDirection( const Vector3D_t& Direction );

		virtual const Vector3D_t&			GetDirection() const;

		// ILight
		virtual void						SetColor( const Vector4D_t& Color );
		virtual void						SetSpecular( const Vector4D_t& Color );
		virtual void						SetIntensivity( float Intensivity );

		virtual const Vector4D_t&			GetColor() const;
		virtual const Vector4D_t&			GetSpecular() const;
		virtual float						GetIntensivity() const;

		// DirectionalLight
		DirectionalLight();
		~DirectionalLight();

	private:
		float			intensivity;

		Vector3D_t		direction;
		Vector4D_t		color;
		Vector4D_t		specular;
        UInt32_t        countReferences;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !DIRECTIONALLIGHT_H

