//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SPRITE_H
#define SPRITE_H

#include "engine/isprite.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class Sprite : public ISprite
    {
    public:
        // ISprite
        virtual bool                    Initialize( const Vector2D_t& Size, IMaterial* Material, SPRITE_TYPE SpriteType = ST_SPRITE_ROTATING );

        virtual void                    SetMaterial( IMaterial* Material );
        virtual void                    SetSize( const Vector2D_t& Size );
        virtual void                    SetType( SPRITE_TYPE SpriteType );

        virtual bool                    IsCreated() const;
        virtual IMaterial*              GetMaterial() const;
        virtual IMesh*                  GetMesh() const;
        virtual const Vector2D_t&       GetSize() const;
        virtual SPRITE_TYPE             GetType() const;
        virtual Matrix4x4_t             GetTransformation( ICamera* Camera );

        // ITransformable
		virtual void					Move( const Vector3D_t& FactorMove );
		virtual void					Rotate( const Vector3D_t& FactorRotate );
		virtual void					Rotate( const Quaternion_t& FactorRotate );
		virtual void					Scale( const Vector3D_t& FactorScale );

		virtual void					SetPosition( const Vector3D_t& Position );
		virtual void					SetRotation( const Vector3D_t& Rotation );
		virtual void					SetRotation( const Quaternion_t& Rotation );
		virtual void					SetScale( const Vector3D_t& Scale );

		virtual const Vector3D_t&		GetPosition() const;
		virtual const Quaternion_t&		GetRotation() const;
		virtual const Vector3D_t&		GetScale() const;
		virtual const Matrix4x4_t&		GetTransformation();

        // Sprite
        Sprite();
        ~Sprite();

    private:
        void				UpdateTransformation();

 		bool				isNeedUpdateTransformation;

        SPRITE_TYPE         type;
        Vector2D_t          size;
        IMesh*              mesh;

		Vector3D_t			position;
		Quaternion_t		rotation;
		Vector3D_t			scale;
		Matrix4x4_t			transformation;       
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SPRITE_H