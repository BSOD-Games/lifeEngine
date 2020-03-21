//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ILIGHT_H
#define ILIGHT_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class ILight : public IReferenceObject
    {
    public:
        virtual ~ILight() {}
        virtual void				SetColor( const Vector4D_t& Color ) = 0;
        virtual void				SetSpecular( const Vector4D_t& Color ) = 0;
        virtual void				SetIntensivity( float Intensivity ) = 0;

        virtual const Vector4D_t&                   GetColor() const = 0;
        virtual const Vector4D_t&                   GetSpecular() const = 0;
        virtual float				GetIntensivity() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ILIGHT_H

