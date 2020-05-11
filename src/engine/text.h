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

#include <string>

#include "engine/itext.h"
#include "studiorender/imaterial.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IMaterial;
    class IStudioRenderTechnique;
    class IStudioRenderPass;
    class IShaderParameter;
    class ITexture;

    //---------------------------------------------------------------------//

    class Text : public IText
    {
    public:
        // IReferenceObject
        virtual void                    IncrementReference();
        virtual void                    DecrementReference();
        virtual void                    Release();
        virtual UInt32_t                GetCountReferences() const;

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
        virtual IMesh*                  GetMesh();

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
        void                UpdateGeometry();
        void                Delete();

 		bool				isNeedUpdateTransformation;
        bool                isNeedUpdateGeometry;

        IMesh*                      mesh;
        IFont*                      font;
        std::string                 text;
        UInt32_t                    characterSize;
        UInt32_t                    countReferences;
        float                       letterSpacingFactor;
        float                       lineSpacingFactor;
        Vector2D_t                  textureSize;
        Vector3D_t                  color;    

        IMaterial*                  material;
        IShaderParameter*           materialParam_color;
        IShaderParameter*           materialParam_basetexture;

		Vector3D_t			        position;
		Quaternion_t		        rotation;
		Vector3D_t			        scale;
		Matrix4x4_t			        transformation;          
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !TEXT_H
