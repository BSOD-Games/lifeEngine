//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TEXT_H
#define TEXT_H

#include "engine/itext.h"
#include "engine/imaterial.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class Text : public IText
    {
    public:
        // IText
        virtual void                    SetFont( IFont* Font );
        virtual void                    SetCharacterSize( UInt32_t CharacterSize );
        virtual void                    SetText( const char* Text );
        virtual void                    SetColor( const Vector3D_t& Color );
        virtual void                    SetLetterSpacingFactor( float LetterSpacingFactor );
        virtual void                    SetLineSpacingFactor( float LineSpacingFactor );

        virtual IFont*                  GetFont() const;
        virtual UInt32_t                GetCharacterSize() const;
        virtual const char*             GetText() const;
        virtual const Vector3D_t&       GetColor() const;
        virtual float                   GetLetterSpacingFactor() const;
        virtual float                   GetLineSpacingFactor() const;
        virtual IMesh*                  GetMesh() const;

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

        // Text
        Text();
        ~Text();

    private:        
        void				UpdateTransformation();

 		bool				isNeedUpdateTransformation;

		Vector3D_t			position;
		Quaternion_t		rotation;
		Vector3D_t			scale;
		Matrix4x4_t			transformation;  
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !TEXT_H