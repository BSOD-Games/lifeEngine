//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISPRITE_H
#define ISPRITE_H

#include "common/rect.h"
#include "engine/itransformable.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IMaterial;
    class IMesh;
    class ICamera;

    //---------------------------------------------------------------------//

    enum SPRITE_TYPE
    {
        ST_SPRITE_STATIC,
        ST_SPRITE_ROTATING,
        ST_SPRITE_ROTATING_ONLY_VERTICAL
    };

    //---------------------------------------------------------------------//

    class ISprite : public ITransformable
    {
    public:
        virtual ~ISprite() {}
        virtual bool                Initialize( const Vector2D_t& Size, IMaterial* Material, SPRITE_TYPE SpriteType = ST_SPRITE_ROTATING ) = 0;

        // TODO: Implement this function with help MaterialProxy
        // virtual void                 SetTextureRect( const FloatRect_t& Rect ) = 0;
        // virtual const FloatRect_t&   GetTextureRect() const = 0;

        virtual void                SetMaterial( IMaterial* Material ) = 0;
        virtual void                SetSize( const Vector2D_t& Size ) = 0;
        virtual void                SetType( SPRITE_TYPE SpriteType ) = 0;

        virtual bool                IsCreated() const = 0;
        virtual IMaterial*          GetMaterial() const = 0;
        virtual IMesh*              GetMesh() const = 0;
        virtual const Vector3D_t&   GetMin() = 0;
        virtual const Vector3D_t&   GetMax() = 0;
        virtual const Vector2D_t&   GetSize() const = 0;
        virtual SPRITE_TYPE         GetType() const = 0;
        virtual Matrix4x4_t         GetTransformation( ICamera* Camera ) = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define SPRITE_INTERFACE_VERSION "LE_Sprite001"

//---------------------------------------------------------------------//

#endif // !ISPRITE_H
